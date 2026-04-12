1. data race: nhiều thread cùng truy cập vào 1 biến chung, mà có ít nhất 1 thread có hành động ghi vào
2. data condition: nhiều thread cùng chạy 1 đoạn mã

3. deadlock: nhiều thread cùng chờ nhau để có thể tiếp tục thực thi, dẫn đến tình trạng tất cả thread đều bị block và không thể tiếp tục thực thi được nữa

std::mutex m1, m2;
void A() {
    std::lock_guard<std::mutex> l1(m1);
    std::lock_guard<std::mutex> l2(m2);
}
void B() {
    std::lock_guard<std::mutex> l1(m2);
    std::lock_guard<std::mutex> l2(m1);
}

->

std::mutex m1, m2;
void A() {
    std::lock_guard<std::mutex> l1(m1);
    std::lock_guard<std::mutex> l2(m2);
}
void B() {
    std::lock_guard<std::mutex> l1(m1);
    std::lock_guard<std::mutex> l2(m2);
}

3.livelock: không bị kẹt cứng như deadlock, nhưng cũng éo làm được việc gì ra hồn.

-> Tức là các thread/process vẫn chạy, vẫn phản ứng, vẫn đổi trạng thái liên tục
nhưng vì cứ nhường nhau hoặc retry sai cách nên không ai tiến lên được.

4. lock-free: là lock là ko cần dùng tới mutex hay khóa nào cả
-> thread chạy liên tục mà ko cần block để đợi khóa, mục đích là với đoạn code nhỏ thì nó performance hơn là dùng lock mutex

ví dụ đơn giản là dùng biến atomic thôi

std::atomic<int> x = 5;
void add_one() {
    int old = x.load();
    while (!x.compare_exchange_weak(old, old + 1)) {
        // thất bại thì old được cập nhật lại,
        // rồi thử tiếp
    }
}
-> như này thì vừa ko race data mà vừa ko race condition
void add_one() {
    int old = x.load();
    x.store(old + 1);
} 
-> cái này thì risk hơn vì load là atomic, store cũng là atomic, như này thì ko bị race data nhưng vẫn có thể bị race condition

4.lock spin: 
Khi thread không lấy được lock, nó không ngủ, không nhường CPU ngay, mà cứ lặp liên tục để kiểm tra:
- lock đã rảnh chưa
- nếu rảnh thì chiếm luôn
Nó “spin” = quay vòng tại chỗ, nên gọi là spinlock.

- critical section rất ngắn, rất nhỏ thì thôi ko cần sleep, vì thực tế 1 core CPU chỉ chạy 1 thread tại 1 thời điểm, và nó lập lịch để chạy nếu dùng nhiều thread
thì việc chuyển từ thread này sang thread kia cũng mất chút chi phí mặc dù nó nhỏ và rất nhanh

std::atomic<int> lock{__ATOMIC_RELEASE};
int counter = 0;

void spin_lock() {
    // set lên __ATOMIC_ACQUIRE và trả về giá trị cũ, nếu giá trị cũ là 
    while (__atomic_test_and_set(&lock, __ATOMIC_ACQUIRE)) {
        // busy wait
    }
}

void spin_unlock() {
    __atomic_clear(&lock, __ATOMIC_RELEASE); -> set về __ATOMIC_RELEASE
}

void* worker(void* arg) {
    for (int i = 0; i < 100000; i++) {
        spin_lock();
        counter++;
        spin_unlock();
    }
    return NULL;
}
---------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------
1. thiết kế thread safe queue
Một queue thực tế thường nên có:
- push(T)
- try_push(T)
- pop(T&)
- try_pop(T&)
- close() hoặc shutdown()
- trạng thái closed
- có thể thêm timeout:
- wait_and_pop
- push_for
- pop_for

Dùng gì để implement ?
std::mutex
std::condition_variable
std::deque<T> hoặc std::queue<T>

#include <mutex>
#include <condition_variable>
#include <queue>

template<class T>
class ThreadSafeQueue {
public:
    bool push (T value){
        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push(std::move(value));
        }   
        cv_.notify_one();
        return true;
    }

    bool wait_and_pop (T& out){
	    std::unique_lock<std::mutex> lock(mutex_);
	    cv_.wait(lock, [&]{return !queue_.empty() || !running;});
        
	    if (!queue_.empty()){
	    	out = std::move(queue_.front());
            queue_.pop();
	    	return true;
	    }
	return false;
}

private:
    std::mutex mutex_;
    std::condition_variable cv_;
    std::queue<T> queue_;
};

tại sao notify_one() khi push, notify_all() khi close ?
với cv_.wait(lock, [&]{return !queue_.empty() || !running;});

Khi producer push 1 item vào queue, chuyện xảy ra là: từ trạng thái queue_.empty() == true -> sang queue_.empty() == false
Lúc này chỉ cần đánh thức 1 consumer là đủ để lấy item đó ra xử lý.

Nếu bạn notify_all() mỗi lần push chỉ 1 item, mà đang có 20 consumer ngủ, thì chuyện gì xảy ra?
cả 20 thằng cùng bị đánh thức -> cùng tranh mutex -> nhưng chỉ 1 thằng lấy được item -> 19 thằng còn lại kiểm tra xong thấy queue lại rỗng -> rồi ngủ tiếp
-> tốn các kiểu

tiếp theo cần biết sự kết hợp của nhều function thread safe chưa chắc đã thread safe, ví dụ nhé
bool empty(); -> có lock trong hàm ok
T pop(); -> -> có lock trong hàm ok
nhưng: 
if (!q.empty()) {
    auto x = q.pop(); -> thread có thể bị sai hành vi, với mảng có thể crash vì thread khác nó pop ra rồi
}
Vì giữa empty() và pop(), thread khác có thể chen vào -> đó là lý do dùng wait_and_pop()