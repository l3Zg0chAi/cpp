Semaphore là cơ chế đồng bộ hóa (synchronization) 
bản chất nó là biến đếm (counter) kiểm soát việc thread có được chạy tiếp hay phải đợi, 
bao nhiêu thread được chạy nếu là counting semaphore
semaphore dùng kiểu event notification nhiều

semaphore có 2 thao tác chính là wait và signal 
sem_wait   : check coutner, nếu sem > 0 thì pass, có thể truy cập tài nguyên, sau đó giảm counter đi 1, nếu sem = 0 thì luồng chời tại đây
{
	atomic
	{
		if(counter > 0)
		{
			counter--;
			return;
		}
		else
			block();
	}
}
sem_signal : tăng count lên 1, nếu có thread nào đang chờ thì cũng là tín hiệu wakeup cho thread đó 
{
counter++

nếu có waiter
    wakeup 1 waiter
}

Mục đích (Purpose) 
Giới hạn số lượng truy cập đồng thời vào một tài nguyên (ví dụ: chỉ cho phép tối đa 3 luồng dùng database cùng lúc). 
Đồng bộ hóa thứ tự thực thi giữa các luồng (luồng A phải chạy xong thì luồng B mới được chạy). 
Tránh race condition (xung đột khi nhiều luồng cùng đọc/ghi dữ liệu). 

Loại              Giá trị counter       Công dụng 
Binary Semaphore  0 hoặc 1              Giống như một cái khóa (lock), gần giống mutex 
Counting Semaphore 0 → N                 Quản lý N tài nguyên giống nhau (vd: connection pool) 


Tiêu chí                  Mutex                                          Semaphore 
Bản chất                  Cơ chế khóa (locking)                          Cơ chế báo hiệu (signaling) 
Giá trị                   Chỉ 0 hoặc 1                                   0 đến N (đếm được) 
Quyền sở hữu (ownership)  CÓ — luồng nào khóa thì chính luồng đó phải mở  KHÔNG — luồng A có thể wait(), luồng B có thể signal() 
Mục đích chính            Bảo vệ vùng tới hạn (critical section)         Đồng bộ + giới hạn số lượng tài nguyên 
Số luồng vào được         Chỉ 1 luồng tại một thời điểm                  Tối đa N luồng 

Ghi nhớ ngắn gọn: 

Mutex = "Cái chìa khóa cửa ra vào" — chỉ 1 người vào, người đó cầm chìa, dùng xong tự trả lại thì người khác mới cầm chìa mở được -> ownership 
Semaphore = "Số chỗ trống trong bãi đỗ xe" — có N chỗ, xe vào thì giảm, xe ra thì tăng; ai vào ai ra cũng được. 

ví dụ 3 chỗ trống trong bãi, 5 xe muốn vào 
#include <stdio.h> 
#include <pthread.h> 
#include <semaphore.h> 
#include <unistd.h> 

sem_t parking;   // Khai báo semaphore cho bãi đỗ xe 

void* car(void* arg) { 
   int id = *(int*)arg; 

   printf("Xe %d: đang chờ chỗ trống...\n", id); 

   sem_wait(&parking);   // (1) Xin một chỗ -> counter giảm 1 
                         //     Nếu hết chỗ (counter=0) thì PHẢI CHỜ
printf("Xe %d: ĐÃ VÀO bãi đỗ.\n", id); 
   sleep(2);             // (2) Giả lập xe đỗ trong 2 giây 

   printf("Xe %d: rời bãi đỗ.\n", id); 
   sem_post(&parking);   // (3) Trả lại chỗ -> counter tăng 1 
                         //     Đánh thức xe đang chờ 

   return NULL; 
} 

int main() { 
   pthread_t t[5]; 
   int ids[5] = {1, 2, 3, 4, 5}; 

   sem_init(&parking, 0, 3);  // (0) Khởi tạo: 3 chỗ trống 

   for (int i = 0; i < 5; i++) 
       pthread_create(&t[i], NULL, car, &ids[i]); 

   for (int i = 0; i < 5; i++) 
       pthread_join(t[i], NULL); 

   sem_destroy(&parking); 
   return 0; 
} 

thực chất nếu code như này 
thread a chạy liện tục 
thread b cũng chạy liên tục 

thread a 
lock mutex -> update dữ liệu -> unlock 
thread b 
lock mutex -> xử lý dữ liệu -> unclock 

Ở pattern này, mỗi thread tự lock và tự unlock → đây là mutual exclusion thuần túy. Và đúng vậy, binary semaphore làm y chang. Khác biệt còn lại chỉ là 2 thứ "ẩn": 

1 Priority Inheritance — nếu A ưu tiên thấp đang giữ khóa, B ưu tiên cao đang chờ: mutex sẽ nâng tạm ưu tiên A để nó nhả khóa nhanh. Semaphore thì không → B có thể bị "kẹt" lâu hơn (priority inversion). Đây là khác biệt runtime thật, không phải lý thuyết suông. 
2 Ownership check — mutex (ở chế độ error-check) sẽ báo lỗi nếu thread không sở hữu mà đòi unlock; semaphore thì cho qua. 
→ Trong môi trường embedded/RTOS, riêng cái priority inheritance đã đủ là lý do thực tế để chọn mutex. Còn nếu app PC bình thường, không quan tâm ưu tiên, thì đúng là "khác gì đâu". 



Tầng 2: Đây mới là điểm quan trọng — code của bạn KHÔNG đồng bộ thứ tự 
Mình đoán ý đồ thật của bạn là: "A cập nhật dữ liệu, B xử lý dữ liệu mà A vừa cập nhật." Đây là bài toán producer–consumer. 

Nếu vậy thì cả mutex LẪN binary semaphore trong đoạn code trên đều SAI — và đây mới là điều cốt lõi: 

lock/unlock chỉ đảm bảo A và B không chạy ĐỒNG THỜI. 
Nó KHÔNG đảm bảo thứ tự "A xong rồi B mới làm". 

Vì cả hai thread chạy liên tục, hệ điều hành có thể xếp lịch: 

A lock-update-unlock 
A lock-update-unlock     <- A chạy 2 lần liên tiếp! 
A lock-update-unlock     <- 3 lần! 
B lock-process-unlock    <- B mới chạy, dữ liệu cũ đã bị ghi đè mất 2 lần 
→ B có thể bỏ lỡ dữ liệu (A ghi đè trước khi B kịp đọc), hoặc xử lý lại cùng một dữ liệu nhiều lần. Mutex không cứu được, vì mutex sinh ra không phải để làm việc này. 

Vậy đúng ra phải dùng gì? 
Tùy ý đồ:
Nếu chỉ cần "không đụng nhau" (không quan tâm thứ tự) → Mutex là đủ và đúng nhất 
Đó chính là pattern bạn viết. Dùng mutex vì có priority inheritance. 

Nếu cần "A báo cho B mỗi khi có data mới" (signaling/ordering) → Dùng semaphore để báo hiệu, KHÔNG phải để khóa 
sem_t data_ready; 
sem_init(&data_ready, 0, 0);   // bắt đầu = 0 (chưa có data) 

// Thread A (producer) 
update_data(); 
sem_post(&data_ready);         // báo: "có data rồi!" 

// Thread B (consumer) 
sem_wait(&data_ready);         // chờ tới khi A báo 
process_data(); 
Để ý điểm khác biệt căn bản so với code của bạn: 

Code của bạn: B lock rồi unlock → B tự khóa tự mở, không ai báo cho ai. B chạy bất kể có data mới hay không. 
Code semaphore: A post, B wait → hai chủ thể khác nhau. B nằm ngủ cho tới khi A đánh thức. Đây chính là cái "không có ownership" mà ta nói ở các phần trước. 
→ Đây mới là chỗ semaphore làm được mà mutex không làm được. Không phải vì cơ chế khóa khác nhau, mà vì vai trò khác nhau: một bên là "khóa", một bên là "tín hiệu". 

Tóm lại câu trả lời cho bạn 
Code của bạn                    (lock/unlock 2 đầu)                               Semaphore báo hiệu (post/wait) 
Mục đích                        Loại trừ — không chạy đồng thời                   Đồng bộ — A xong B mới làm 
Ai lock/unlock                  Mỗi thread tự lock + tự unlock                    A post, B wait (khác chủ thể) 
Đảm bảo thứ tự?                ❌ Không                                          ✅ Có 
B có "ngủ chờ" data không?     ❌ Không, chạy liên tục phí CPU                   ✅ Có, ngủ tới khi có data 

Nên kết luận chính xác là: 
Đúng đoạn code bạn viết → mutex ≈ binary semaphore, khác nhau chủ yếu ở priority inheritance. Bạn nhìn không sai. 
NHƯNG nếu mục đích là "B xử lý dữ liệu A vừa tạo", thì vấn đề không nằm ở chọn mutex hay semaphore — mà code đó thiếu cơ chế đồng bộ thứ tự, và lúc này semaphore (post/wait) hoặc queue/condition variable mới là lời giải đúng. 