Mục lục:
1. Data type, rvalue, lvalue
2. OPERATORS
3. POINTER
4. FUNCTION POINTR
5. NAMESPACE
6. FUNCTION
7. STATIC DISPATH, DYNAMIC DISPATH
8. CLASS
 - VIRTUAL METHOD -> đa hình
 - CONSTRUCTOR 
 - member initializer list
 - Operator Overloading in C++
 - CONST OBJECT
9. STL Library FUNCTOR
10. Extern, liên kết ngoài
11. Exception Handing and Signal handing
12. Casting
13. lambda

==================================================================================================================================================================

Data type: có 2 loại, 1 là các kiểu dữ liệu có sẵn, 2 là kiểu do user define ra, kể cả kiểu con trỏ hàm nhé, dùng typedef là được đấy!

* lvalue và rvalue
- lvalue là 1 expression mà refer tới 1 object, ta có thể get được address, có thể set value tại address đó
- còn rvalue thường là 1 temporary value, ta không thể get được địa chỉ và set giá trị 1 cách trực tiếp cho nó như ta làm với lvalue
đi kèm sẽ có 2 khái niệm là lvalue reference và rvalue reference ?
- lvalue reference thì tham chiếu tới 1 lvalue
và
- rvalue reference tham chiếu tới 1 rvalue
int a = 0; // - a là lvalue và 0 là rvalue
int& b = a; //- b là biến tham chiếu tới a, là reference type int
int&& r = 1+2;
int&& r = 3;
-> it is same
- expression '1+2' creates temporary value - nó là rvalue with value là 3, rvalue reference r này sau đó được khởi tạo refer tới cái rvalue này
- có thể truy cập contents của rvalue(đọc ghi oke) và extend lifetime của rvalue này through rvalue reference, nhớ là thông qua rvalue ref, chứ k phải trực tiếp
- ta có thể dùng cout << &r để in địa chỉ của cái rvalue ref corresponds vói cái rvalue mà nó refer tới, chứ đấy cũng k phải là địa chỉ của original rvalue tạo ra bởi '1+2', vì ta k thể cout << &(1+2), vì cái rvalue nó k có 1 cái gọi là persistent memory: vùng nhớ valid tới cuối chương trình hoặc it nhất là tới lúc nó được hủy đi 1 cách explicit, rõ ràng
- và temporary value được đại diện bởi cái rvalue và được created và destroyed bởi biểu thức '1+2', nhưng như đã nói ở trên, lifetime của nó sẽ được extend bởi thằng rvalue reference

lvalue ref thì chỉ bind tới lvalue
rvalue ref thì chỉ bind tới rvalue
nhưng mà const lvalue ref thì lại bind được cả 2, vậy nên là 1 rvalue ref có thể implicit convert to const lvalue ref

xem ví dụ về constructor:
#include <iostream>
using namespace std;
class MClass{
public:
    MClass(){
        cout << "default contructor" << endl;
        mp = new int;
    }
    MClass(const MClass& tem) : mp{tem.mp}{
        cout << "copy contructor" << endl;
        // tem.mp = nullptr; -> can do that
    }
    MClass(MClass& tem) : mp{tem.mp}{
        cout << "copy contructor" << endl;
        tem.mp = nullptr;
    }
    MClass(MClass&& tem) : mp{tem.mp} {
        cout << "move contructor" << endl;
        tem.mp = nullptr;
    }
    int mx = 10;
    int *mp;
};
int main (){
    cout << "start review !" << endl;
    MClass a;
    *(a.mp) = 20;
    MClass b = a; (1) sẽ gọi copy consructor, vì đối số là 1 lvalue
    MClass b = std:move(a); (2) sẽ gọi move constructor, vì đối số là 1 rvalue ref, nhưng cần thì nó có thể implicit convert tới const lvalue ref 
    cout << "b.mp: " << " " << &(b.mp) << " " << b.mp << " " << *(b.mp) << endl;
    if(a.mp == nullptr){
        cout << "pointer is null" << endl;
    }
    else {
        cout << "a.mp: " << " " << &(a.mp) << " " << a.mp << " " << *(a.mp) << endl;
    }
    return 0;
}
- mình có thể thấy cái copy constuctor với non-const nó có thể làm 1 việc tương tự y hệt như move construcotr, nhưng rõ ràng là sai ý nghĩa của copy vì copy xong mà cho thằng cũ null mẹ luôn, ý là k dùng nữa
- copy xong là còn dùng thằng cũ, còn move xong là k dùng thằng cũ nữa
- std:move() nó trả về 1 rvalue ref tới thằng a, và nó có thể implicit convert to const lvalue ref nên nó thỏa mã để gọi đến copy constructor với tham số const, nhưng khi đấy thì đâu có thể change được cái gì nữa đâu, move xong mà thằng cũ vẫn còn nguyên
-> cái cơ chế move sẽ là: gán 1 thằng vào 1 thằng đã có then cho thằng đã có về null -> đỡ tốn time phải tạo memory cho thằng mới rồi copy sang

sử dụng rvalue reference hay std::move giúp ta loại bỏ được thao tác cấp phát, copy dữ liệu , hủy object, nói chung là kha khá performance

==================================================================================================================================================================

OPERATORS

- về unary operator nó là toán tử 1 ngôi, nghĩa là chỉ cần 1 toán hạng để sử dụng được, ví dụ như ++ hay --
- về binary operator thì nó hoạt động trên 2 toán hạng, đơn giản như ==, !=, >, <...có 1 loại đặc biệt là toán tử bit(&, |, ^), các toạn hạng của nó sẽ phải chuyển
sang kiểu nhị phân 1 với 0, và ghép chúng lần lượt theo toán tử để ra kết quả
- toán tử logic: &&, ||
- toán tử 3 ngôi, ví dụ ((a>b?a:b)>c)? (a>b?a:b):c, ban đầu so sánh a vs b, sau đó so sánh vs c

- tham chiếu thì khai báo kèm theo &, hiểu đơn giản nó như 1 cái tên thứ 2 cho 1 cái biến đã tồn tại, k phải là 1 biến mới
=> phải khởi tạo nó tham chiếu đến 1 biến chứ không được khai báo rồi để đó, nó sẽ lỗi(con trỏ thì không)

- khi truyền tham số theo dạng tham chiếu đến hàm thì nó sẽ tránh việc tạo ra 1 đối tượng tạm như việc truyền tham trị, đồng nghĩa nó cũng có khả năng thay đổi giá trị
của thằng gốc truyền đến -> để chắc chắn thì ta nên thêm const vào, nhưng khi đó phải chú ý kiểu return về, khi tham số là const & thì k thể return về kiểu tham chiếu,
lý do cũng giống như nếu tham số là kiểu tham chiếu thì k được truyền hằng số vào

- nó khác con trỏ ở chỗ, con trỏ nắm địa chỉ và có thể trỏ đi được chỗ khác, còn nó bất di bất dịch vì đơn giản nó chính là cái biến nó tham chiếu đến, có vẻ 1 tham chiếu 
nó sẽ giống với Type *const pointer;

==================================================================================================================================================================

Con trỏ thường: POINTER

void* thì sẽ trỏ tới mọi vùng nhớ kiểu khác được, lúc đó ta chỉ cần ép kiểu thôi

pointer=p
- phân biệt type* p, const type* p, type* const p, const type* const p
- ex        int*   , const int*   , int* const   , const int* const
Note: const type = type const

- khi 1 con trỏ trỏ đến 1 vùng nhớ thì nó sẽ nắm giữ 2 thứ của vùng nhớ đó
hoặc kể cả con trỏ trỏ đến 1 con trỏ khác thì cũng thế -> có nghĩa là nó sẽ nắm giữ được 2 thứ của cái nó trỏ đến 

 ***                                                       ***
***  con trỏ nó nắm giữ 2 thứ, 1 là địa chỉ, 2 là giá trị !  ***
 ***                                                       ***

=> cái việc khai báo con trỏ như thế nào sẽ quyết định việc có thể thao tác 2 thứ trên như thế nào

- type* p thì p là 1 con trỏ trỏ đến 1 vùng nhớ kiểu type, có thể trỏ sang 1 vùng nhớ khác, nắm giữ địa chỉ của vùng nhớ đó. có thể thay đổi giá trị của vùng nhớ đó 
thông qua con trỏ
- const type* p thì p là con trỏ trỏ đến vùng nhớ kiểu const type, or kiểu type cũng được, cũng có thể trỏ sang vùng nhớ khác, nhưng không thể thay đổi giá trị vùng 
nhớ đó thông qua con trỏ
- type* const p thì p là 1 hằng con trỏ, không trỏ đi chỗ khác được, mean trỏ đến 1 vùng nhớ duy nhất kiểu type, có thể thay đổi giá trị thông qua con trỏ
- const type* const p thì p là 1 con trỏ hằng trỏ đến 1 hằng giá trị, vẫn có thể trỏ đến 1 giá trị không hằng nhưng ý nghĩa ở đây là nó không thể trỏ đi chỗ khác cũng 
k thể thay đổi giá trị nó trỏ đến thông qua nó

chốt lại:
- type* p: thoải mái
- const type* p: không thay đổi được giá trị thông qua con trỏ == type const*
- type* const p: không trỏ đi chỗ khác được

=> NHẬN RA: * sẽ là cái nhận biết const sẽ aplly cho cái nào, trước * thì apply cho kiểu trỏ đến (hằng hay ko), sau * thì apply cho con trỏ(con trỏ hằng hay là ko)

==================================================================================================================================================================

* Con trỏ thông minh: Smart Pointer
std::unique_ptr
- small, fast, quản lý độc quyền 1 resource, không thể copy sang unique_ptr khác, chỉ có thể chuyển rvalue sang, convert sang share_pointer cũng thế -> use std::move
chuyển sang rvalue chính là dùng std::move
- std::unique_ptr<class> ob(new class);
- std::unique_ptr<class> ob1 = std::move(ob); \\ dùng std::move chuyển sang rvalue
- std::shared_ptr<class> ob2 = std::move(ob); \\ dùng std::move chuyển sang rvalue, nói chung là phải chuyển sang rvalue
- bình thường nó sẽ xóa resource bằng delete, custom deteler thì dùng bằng lambda được, lúc đó thì mình phải tự thực hiện delete trong biểu thức custom delete
- custom deleter is part of unique pointer type

EXAMPLE:
auto del1 = [](mclass* mob){
    cout << "delete custom 1" << endl;
    delete mob;
};
auto del2 = [](mclass* mob){
    cout << "delete custom 2" << endl;
    delete mob;
};
class mclass;
std::unique_ptr<mclass, decltype(del1)> obunique1(new mclass, del1); ->      type là <mclass, decltype(del1)>
std::unique_ptr<mclass, decltype(del1)> obunique2 = std::move(obunique1); -> type là <mclass, decltype(del1)>
// 2 cái sau sẽ wrong, vì khi kiểu của thằng obunique1 đã fix cứng, del là del1 - vì deleter là 1 phần của pointer type
=> thay deleter là coi như là type khác

std::unique_ptr<mclass, decltype(del2)> obunique2 = std::move(obunique1); -> type là <mclass, decltype(del2)>
std::unique_ptr<mclass> obunique2 = std::move(obunique1); -> cho nên đâu thể convert type từ <mclass, decltype(del1)> gắn sang <mclass, decltype(del2)>

/*------------------------------------------------------------------------------------------------------------------------*/

std::shared_ptr: share object own ship bởi nhiều đối tượng share_ptr với nhau
- quản lý thông qua 1 biếm đếm gọi là reference count - nằm trong control block
- thông thường tăng giá trị biến đếm thông qua constructor của std::shared_ptr và giảm giá trị thông qua destructor,
copy assignment sẽ làm cả 2 (s1 = s2, giảm của s1 tăng của s2)

std::shared_ptr<mclass> ob1(new mclass{});
std::shared_ptr<mclass> ob2(ob1); -> tăng reference count

but std::shared_ptr<mclass> ob2(std::move(ob1)); -> dont tăng reference count, vì thằng ob1 về null rồi nghĩa là nó k trỏ tới object nữa r

- reference count về tới 0, the last std::shared_ptr sẽ destroy cái object mà nó point to
- size gấp đôi size của raw pointer: vì cơ bản có thể hiểu gồm share pointer gồm có 2 "parts", 1 là raw pointer quản lý object, 2 là khối control block được quản lý nội bộ bởi đối tượng share pointer
- memory cho reference count phải được cấp phát động
- tăng giảm cho reference count phải là atomic
- support custom deleter, but not like unique pointer, - custom deleter is not part of unique pointer type
    auto customDelete1= [](mclass* ob){
        cout << "customDelete1" << endl;
        delete ob;
    };

    auto customDelete2= [](mclass* ob){
        cout << "customDelete2" << endl;
        delete ob;
    };
    std::shared_ptr<mclass> ob4(new mclass{}, customDelete1); -> nó tạo 1 control block
    std::shared_ptr<mclass> ob5(new mclass{}, customDelete2); -> cũng tạo 1 control block
    ob4 = ob5; -> delete object được trỏ bởi ob4, và line code này có thể thấy custom delete k phải là a part of type của share pointer
    while(true){}

nhưng nếu
    std::shared_ptr<mclass> ob4(new mclass{}, customDelete1); ->  nó tạo 1 control block
    std::shared_ptr<mclass> ob5(ob4); -> sẽ k cũng tạo 1 control block
    ob4 = ob5; -> sẽ k có gì xảy ra cả
và yeah, custom deleter sẽ k làm tăng size của share pointer, chắc chắn thì nó cũng cần memory để lưu trữ nhưng sẽ k phải là share_ptr
- control block sẽ chứa reference count/ weak pointer/ copy of custom deleter allocator ...
- share pointer phải tạo từ 1 share pointer, nếu tạo từ raw pointer thì nó sẽ có control block khác-  hiểu như control block được tạo ra từ thằng share pointer đầu tiên
nếu tạo share pointer thông qua raw resource
- dùng make_shared thì k thể dùng được custom deleter

- truyền con trỏ this sẽ được tính là raw pointer, khi này dùng public std::enable_shared_from_this<Widget>
ví dụ:

#include header
class Widget;
std::vector<std::shared_ptr<Widget>> processedWidgets;
class Widget {
public:

    void process(){
        processedWidgets.emplace_back(this);
        for(int i = 0; i<listmclass.size(); i++){
            cout << "inside vector: " << listmclass[i].use_count() << endl;
        }
    }
    …
};

int main (){

    std::shared_ptr<Widget> sh_pt_1(new Widget{});
    sh_pt_1->process();

    std::shared_ptr<Widget> sh_pt_2(sh_pt_1);
    cout << "hailn4" << endl;
    sh_pt_2->process();

    cout << "int fact: " << sh_pt_2.use_count() << endl;

    std::shared_ptr<Widget> sh_pt_3(sh_pt_1);
    cout << "hailn4" << endl;
    sh_pt_3->process();

    cout << "int fact: " << sh_pt_2.use_count() << endl;

    return 0;
}

output:
default contructor
inside vector: 1
hailnm4
inside vector: 1
inside vector: 1
int fact: 2
hailnm4
inside vector: 1
inside vector: 1
inside vector: 1
int fact: 3
destructor
destructor
destructor
destructor

chỗ này là vì nó coi this là raw pointer, và khi truyền vào vector, nó tạo ra 1 share pointer khác trong vector từ this -> có 1 control block khác
mới luôn, nên nhìn thấy count luôn bằng 1 trong vector, lúc hủy cũng có tận 4 thằng: 1 thằng ban đầu và 3 thằng create ra trong vector

phải dùng như sau:
class mclass : public std::enable_shared_from_this<Widget> {
...
}
    void process(){
        processedWidgets.emplace_back(shared_from_this());
    }
-> nó tạo share pointer trong vector và nó đồng bộ với share pointer bên ngoài vector luôn, chung control block
default contructor
inside vector: 2
hailnm4
inside vector: 4
inside vector: 4
int fact: 4
hailnm4
inside vector: 6
inside vector: 6
inside vector: 6
int fact: 6
destructor

có thể thấy chỉ có 1 destructor, và ref count là luôn x đôi kìa

/*------------------------------------------------------------------------------------------------------------------------*/

std::weak_ptr
- thường được tạo ra từ shared pointer, nhưng k ảnh hưởng tới biến đếm
- sử dụng khi cần dùng bộ nhớ đệm cache, observer list(observer pattern), tránh vòng lặp shared pointer

*** Why use make_unique and make_shared instead of use direct new 

*** khác biệt giữa dùng new và make function như make_unique, make_shared:
- thứ 1 là về phân bổ vùng nhớ, create share_ptr bằng cách dùng trực tiếp new sẽ tạo ra 2 memory area riêng biệt 1 là cho object, 2 là cho control block, memory cho 
object có thể được released trước memory cho control block, còn dùng make_share thì k thể, vì dùng make shared thì chỉ có 1 vùng nhớ chung cho cả 2 tk này, explain: 
trong control block, ngoài reference count còn có weak count - coi như là đếm số weak pointer trỏ tới, thì memory cho control block chỉ bị hủy khi cả reference count 
và weak count đều về 0, còn memory cho objetc hủy khi chỉ reference count về 0

- thứ 2 là về safe - exception, ví dụ có biểu thức gọi function như sau: 
processWidget(std::shared_ptr(new Widget, cusDel), computePriority()); 
thì C++ nó không có thứ tự để đánh giá biểu thức, shared pointer sẽ gồm create Widget trên heap, sau đó run contructor của shared_ptr sẽ nắm được cái vùng heap đó của 
Widget, rồi chạy function computePriority, nhưng nếu computePriority chạy sau bước new Widget và trước bước contructors của shared và computePriority  xảy ra 1 exception, 
đối tượng shared_ptr k nắm được new Widet trả về -> leak
-> dùng make_shared, nếu có custom deleter k dùng được make shared, thì tạo shared pointer trước rồi mới truyền vào hàm

- 1 điểm nhỏ về share_ptr và weak_ptr
ta có 1 ví dụ như sau:

class A{
    public:
    A(){cout << "constructor" << endl;}
    ~A(){cout << "destructor" << endl;}
};

std::weak_ptr<A> func(){
    std::shared_ptr<A> SP(new A);
    std::weak_ptr<A> WP;
    WP = SP;
    return WP;
}

int main()
{
    func(); (1)
    std::weak_ptr<A> WP = func(); (2)
    while(true){}
    return 0;
}

=> dù là gọi theo cách (1) hay (2) thì ta sẽ đều thấy object bị hủy, do đó ta có thể chắc chắc khi 1 object được quản lý bởi shared_ptr, nó sẽ bị delete khi count của 
shared_ptr = 0, nghĩa là thằng shared_ptr cuối cùng giữ nó bị hủy, nếu có bất kì weak_ptr nào cũng trỏ vào đó thì cũng k giữ được life cho object.
Trong TH trên, nếu sử dụng raw pointer, gọi theo cách 1 sẽ bị leak vì khi đó k còn con trỏ nào giữ object đó nữa, gọi theo cách 2 thì ta sẽ không thấy object bị hủy trước 
khi main kết thúc vì while(true)

==================================================================================================================================================================

FUNCTION POINTR

Con trỏ hàm: là con trỏ trỏ tới 1 hàm, k có cấp phát vùng nhớ gì cả, giữ địa chỉ tại đoạn mã bắt đầu chạy hàm, khi khai báo cần xác định kiểu return của hàm, tham số truyền, 
tên con trỏ phải đặt trong ()
- khi truyền địa chỉ hàm cho con trỏ hàm không cần &, vì đơn giản, bản thân hàm cũng kiểu như là 1 địa chỉ, 1 con trỏ hàm mẹ rồi. viết mỗi tên thôi nhé
- parameter default của hàm không thể dùng cho con trỏ hàm, vì parameter default của hàm được compiler xác nhận tại thời điểm compile, còn function pointer thì tại thời điểm 
chạy, nên vẫn phải truyền đủ parameter

ví dụ: 
int function(int x, int y){return ...}
int (*FPointer)(int,int) -> con trỏ hàm có thể trỏ tới mọi hàm có kiểu return là int và 2 tham số là int
FPointer = function; // right, not function() or not *FPointer , gán địa chỉ hàm tới con trỏ
(*FPointer)(9,3); -> gọi hàm

Định nghĩa 1 kiểu con trỏ hàm để dùng 
typedef int (*FPointer)(int,int) -> FPointer là kiểu con trỏ hàm có return là int và 2 tham số là int
FPointer x = function; -> gán địa chỉ
(*x)(9,3);

- con trỏ hàm cũng có thể dùng làm tham số, nhiều TH nó là callback function đó, tham số là tham số, hiểu ko ta, default parameter ? ok luôn

function2(int x, (int)(*FPointer)(int,int) = function){ (*FPointer)(int,int) } -> khi gọi function2 mà k truyền hàm vào thì nó mặc định chạy hàm fucntion
or
function2(int x, FPointer y = function){ (*y)(int,int) }

=================================================================================

NAMESPACE

source file sẽ được bộ tiền xử lý duyệt trước, sau đó mới biên dịch sorce code thuần mà k bao gồm các chỉ thị có # nữa

dùng namespace để ta tách biệt đoạn code đó trong 1 scope mà k lo bị trùng lặp tên các biến các hàm ở 1 nơi khác,
khi mà 2 namespace có cùng tên thì thực chất chúng là 1,thì khi đó ko được trùng tên tên các biến các hàm, khi mà truy xuất các thằng trong 
namespace ta dùng toán tử ::, namespace khác class, bản thân nó chỉ là 1 nơi để gom lại trong 1 scope, để phân biệt, còn class là 1 data type

==================================================================================================================================================================

FUNCTION

- inline function là hàm nội tuyến, khi mình đặt từ khóa inline trước hàm thì nó yêu cầu complier thực hiện, nội tuyến nghĩa là nó sẽ coi như ta viết trực tiếp 
nó vào mã mà ko phải là gọi hàm,  còn constexpr function thì nó là yêu cầu trinh biên dịch biên dịch cái hàm đó tại thời gian biên dịch, thì lúc chạy chương trình nó sẽ nhanh hơn

- tham số mặc định của hàm phải đứng sau các tham số k mặc định

- hàm thì ta sẽ có kiểu trả về, tên hàm, các tham số truyền đến hàm và thân hàm, thì tham số truyền vào hay kiểu return về có thể là tham trị, tham chiếu hay con trỏ

return void

return giá trị thì nó sẽ create ra đối tượng tạm thời để lưu cái giá trị đó rồi sau đó sẽ destroy cái đối tượng đó luôn, mà bh complier nó tối ưu nó dùng cơ chế m chứ 
không thấy cần tạo đối tượng tạm nữa

return con trỏ hoặc return tham chiếu cần chú ý cái đối tượng mình chọc vào, là biến cục bộ thì oẳng chó

chú ý khi return về kiểu tham chiếu thì đừng nhìn thấy symbol & mà nói là địa chỉ nhé, ví dụ tham chiếu của 1 biến int khác hoàn toàn với địa chỉ của 1 biến int

int& x = y; nó khác hoàn toàn với int* x = &y;
x này vẫn là kiểu int             x này là kiểu int* mẹ rồi   

==================================================================================================================================================================

STATIC DISPATH, DYNAMIC DISPATH trong c++

mình nghĩ sẽ clean: cái vpointer được tạo ra ở private của mỗi class đo đó dùng vtable là khác nhau trỏ đến vtable khác nhau đối với mỗi clas -> nó phải như vậy

(link xịn: https://cppdeveloper.com/c-nang-cao/virtual-tables-vtable-trong-c/)

- trước hết thì hãy hiểu static dispath là gì: trong case này có thể hiểu static dispath là việc tìm đúng địa chỉ của hàm cần gọi, trong case bình thường, khi không 
dùng virtual method, thì khi chúng ta định nghĩa 1 hàm trong class thì lúc biên dịch thằng compiler nó sẽ lưu địa chỉ hàm và dùng để jump đến cái địa chỉ này để chạy hàm 
mỗi khi hàm được gọi, là địa chỉ của dòng lệnh đầu tiên trong hàm thì phải (nhân tiện đây cũng là lý do mình có thể dùng con trỏ hàm)

- lưu ý là mỗi hàm trong class sẽ có 1 địa chỉ tương ứng và là DUY NHẤT tồn tại và được sử dụng chung cho tất cả các đối tượng trong class đó 

OK, dễ hiểu vlìn ?, bây giờ nói về vtable, vpointer and dynamic dispatch

- á đù, trước tiên là lòi ra cái dynamic dispath: thì như nào ? là define ra 1 cái hàm, và nó có địa chỉ, thì lúc biên dịch, compiler xác định được luôn cái địa chỉ đó của 
hàm thì quá trình đó gọi là static dispath. Ngược lại, nếu nó không thể xác định trước được địa chỉ của hàm tại lúc biên dịch, mà phải xác định tại runtime, thì đó là dynamic dispath, 
trường hợp này sảy ra đối với hàm ảo - virtual method, 

clean hơn 1 chút này: giữa class con và class cha thì k thể nạp chồng function, nó chỉ có thể định nghĩa lại, ta chỉ có thể nạp chồng function trong cùng 1 class -> đây chính là đa hình tĩnh

ví dụ nhá:

class A{
public:
virtual method1();
virtual method2();
}
class B : public A{
public:
method1();
}

-> đã áp ụng đa hình rồi đúng không ! thì:
A* = new B();
A->method1();

-> nếu compiler nó áp dụng static dispath vào đây, nghĩa là nó xác định địa chỉ của hàm A::method1 luôn từ lúc biên dịch, thì chỗ này hàm A::method1 phải được chạy, nhưng rõ ràng ở đây là đã chạy hàm B::method1 -> chương trình đã phải tìm địa chỉ của hàm cần chạy ở runtime, và do đó, quá trình này được gọi là dynamic dispath

- bây giờ là đến thằng vtable hay đầy đủ là virtual table: có thể coi thằng vtable này là mảng con trỏ hàm nhỉ , là nó đó ???

khi biên dịch code, với mỗi class mà có chứa virtual method thì nó sẽ tạo ra 1 cái gọi là virtual table, tỉ như ex trên thì nó sẽ tạo ra 2 vtable cho 2 class là A và B, vì class B public lại class A và define lại cái hàm  method1 nên dù mình k có từ khóa virtual ở đó thì nó vẫn là virtual method bình thường thôi.

- thì cái vtable nó sẽ lưu địa chỉ của các hàm virtual mà có thể được gọi thông qua đối tượng của class đó, các phần tử của vtable có thể trỏ đến địa chỉ của hàm virtual mà được định nghĩa bởi chính class đó, hoặc trỏ đến hàm virtual của class cha nếu bản thân nó k override lại

theo ex bên trên, nó sẽ tạo ra 2 vtable riêng cho class A và class B

vtable cho class A chứa 2 con trỏ hàm 1 trỏ đến A:method1 và 1 trỏ đến A::method2

vtable được tạo ra ở class con là B cũng có 2 con trỏ hàm 1 trỏ đến B::method1 và 1 trỏ đến A::method2

- CHÚ Ý là chỉ có 1 vtable DUY NHẤT cho mỗi class và được share dùng chung cho tất cả object được tạo ra của class đó

- tiếp tục là đến thằng Vpointer: là 1 biến thành viên của class thôi, ơ mà được tạo ngầm đấy, không nhìn thấy đâu :))

là như sau: khi biên dịch chương trình, thì class có virtual method thì compiler nó tạo ra 1 vtable cho class đó, đồng thời là nó cũng tạo thêm 1 biến thành viên cho class là vpointer, đồng nghĩa với việc mỗi object được tạo ra sẽ có 1 biến vpoiner như bao biến khác, làm tăng size của object thêm sizeof(vpointer) bytes -> 8bytes vì size của con trỏ luôn bằng 8

vtable là 1 cái mà program nó tạo ra cho vpointer cuả class trỏ tới, chứ nó k nằm trong class, nó có mỗi liên hệ mật thiết với class chứ k phải là của hay là nằm trong class

và đây là cách mà dynamic static hoạt động: trong runtime, 1 lời gọi đến 1 hàm ảo trên 1 đối tượng thì vpointer của đối tượng đó sẽ được sử dụng để tìm vtable tương ứng của class, sau đó trỏ tới đúng function cần gọi

-> note: vpointer thường được tạo ra ở private của class, nên mình sẽ thiên về việc hiểu mỗi 1 class sẽ có 1 vpointer riêng 
=> trả lời câu hỏi, 1 class có virtual function thì size ít nhất bằng 8, vì nó tồn tại ít nhất 1 biến thành viên là vpointer

- Vậy, đã có vài lần build mà bị bệnh là undefined reference to `vtable for MethodXXX

có nghĩa là sao nhỉ ? 1 là thằng vpointer hỏng 2 là thằng phần tử bên trong vtable trỏ đến hàm MethodXXX hỏng

thế lỗi "undefined reference to `vtable for MethodXXX" thì cuối cùng là lỗi do vpointer không trỏ được đến vtable hay do thằng phần tử của vtable k trỏ được đến hàm hay trình biên dịch fail hay như thế nào nhỉ :))

==================================================================================================================================================================

CLASS 

- class như 1 cái bộ khung thôi, khi tạo xong 1 cái class thì cũng k có vùng nhớ hay gì cấp phát đi đâu cả, nó là 1 cái bộ khung chung để xây dựng nên các object thôi, giống kiểu bản vẽ nhà 3d với việc xây dựng lên ngồi nhà ấy thật sự vậy !

4 tính chất: đa hình, kế thừa, đóng gói, trừu tượng
nói về kế thừa và đa hình:
kế thừa: thì sẽ có 3 kiểu kế thừa luôn, là public, private, protected, 3 cái phạm vi truy cập cho các thành phần được kế thừa trong class con

phạm vi private của 1 class là bảo vệ, nghĩa là không thể truy xuất nó từ bên ngoài class, do đó kế thừa thì cũng k thể truy xuất được tại class con , nhưng có thể sử dụng bằng cách làm cái function get nó tại class base, dùng tại class con thì ok

public: public->public, protected->protected, 
protected: public, protected -> protected 
private: public, protected -> private 

-> có 3 kiểu kế thừa là như vậy

và sẽ có 2 kiểu đa hình: tĩnh - compiler time, động - runtime
đa hình tĩnh chính là việc nạp chồng hàm trong trong class
đa hình động là cách dùng virtual đấy

- 1 con trỏ lớp cha luôn có thể trỏ  đến 1 đối tượng lớp con, cái việc dùng con trỏ là muốn thao tác với giá trị, hay là cái vùng nhớ, hay là cái dữ liệu mà con trỏ nó trỏ vào, nhưng ở đây nếu k dùng đa hình

- VIRTUAL METHOD
class A;
class B : public A

A* x = new B;
- thì x sẽ không thao tác được các tài nguyên trong B mà chỉ thao tác được trong bản thân nó là A, means nó ưu tiên kiểu dữ liệu con trỏ chứ k ưu tiên kiểu dữ liệu của đối tượng được trỏ
- nạp chồng hàm + virtual -> đa hình: mà khi dùng đa hình rồi nó sẽ giải quyết được vấn đề bên trên

- class con chỉ có thể định nghĩa lại cái hàm đã khai báo ở class cha chứ không thể nạp chồng hàm của class cha tại class con, (giống return, giống tên hàm, giống tham số)

- việc dùng đa hình giải quyết được cái vấn đề là 1 con trỏ hoặc tham chiếu kiểu base trỏ vào 1 kiểu con !

- member initializer lists: là khởi tại danh sách thành viên, nó sẽ khởi tạo theo thứ tự mình khai báo các biến, lưu ý là KHAI BÁO vs KHỞI TẠO có 1 vài điểm khác nhau !

Đã biết là khai báo với khởi tạo là khác đúng không, thì việc mà mình khai báo, xong gán giá trị ban đầu ở constructor nó khác với khởi tạo giá trị ban đầu (nhìn biến const là rõ liền), tỷ như:

class A{
int x; // khai báo x, giá trị rác
A(){
x = 0; // gán giá trị
}
}

khởi tạo hiểu đúng chắc nên là:

class A{
int x = 9; // khởi tạo x = 9
A(){}
}

tương đương với:

class A{
int x;
A() : x{9}
{}
}
-> cũng là khởi tạo x = 9 y như bên trên

ví dụ class có 1 member là const int chẳng hạn, thì phải khởi tạo giá trị ngay chứ k thể khai báo rồi để đấy, nếu k dùng initializer lists thì all object của class đều có 1 biến const int đó với giá trị giống nhau, chẳng thà đặt static mẹ đi, cơ mà mình muốn mỗi object có 1 const int đó với giá trị khác nhau, lúc này cần dùng initializer lists rồi

class A{
public:
A(int x) : mx{x}
{}

private:
const int mx;
}

rõ ràng mỗi 1 lần create ra 1 object of A thì sẽ sử dụng được 1 giá trị mx khác nhau, lưu ý chỗ này là tạo ra rồi thì không có thay đổi được nữa đâu
- trình tự khởi tạo các data member không phụ thuộc vào thứ tự chô mem list init, mà phụ thuộc vào lúc mình khai báo cái biến thành viên đó, từ trên xuống từ trái sang phải !

- nếu có biến thành viên là tham chiếu, kiểu int& ma, bản chất tham chiếu là phải khởi tạo chứ k thể khai báo xong để đó, nếu khi instance 1 object nó sẽ lỗi ngay, nhưng dùng member initializer lists
thì nó lại hợp lý luôn, ngon lành, và phải đảm bảo được vòng đời của biến nó refer tới

- CONSTRUCTOR của class thì như nào !
-> quyết định xem liệu object có được tạo hay không, k có contructor nào phù hợp thì k tạo được object

vài loại như sau: constructor mặc định (có define hoặc ẩn), constructor có tham số, copy constructor (có define hoặc ẩn) và move constructor, cũng có thể nạp chồng constructor bình thường vì đơn giản khi đó là có nhiều cách để tạo ra object !

nói về copy constructor đi: sẽ được sử dụng khi ta gán 1 object cho 1 object, ví dụ
class A{
public:
int x;
int* p;
}

A x; // gọi constructor default để create x
A y = x; // gọi copy constructor default
hiện tại thì thấy k sao đúng ko, vì build và run bình thường, nhưng khổ 1 chỗ, default copy contructors hiểu nôm na nó sẽ nhân bản thằng y từ thằng x, vậy nó sẽ nhân bản ra 2 con trỏ int* p khác nhau đúng ko, nhưng khổ 1 cái, vùng nhớ mà nó trỏ vào thì k có tự nhân bản ra được
nó vẫn là 2 con trỏ khác nhau đấy nhưng bây giờ dùng chung 1 vùng nhớ,nên sau đó sẽ phải tách ra bằng cách cho 1 trong 2 nó trỏ đi chỗ khác là xong, hoặc làm điều đó ngay khi khởi tạo đối tượng bằng copy constructor -> ĐÚNG

-> giải quyết nó bằng cách sử dụng copy constructor mình define ra: A(const A& x){x.ptr = new int();} -> ok, sẽ không bị dùng chung 1 vùng nhớ nữa
A y = x; lúc này sẽ create đối tượng bằng cách gọi A(const A& x) <=> y.A(x) [khởi tạo y từ x]

NOTE: nếu không define copy contructor, biến k phải con trỏ sẽ bình thường(được gán giá trị luôn), biến con trỏ sẽ bị chung chạ như giải thích ở trên,
còn 1 khi đã define copy contructor, muốn làm gì thì phải làm trong hàm, nếu ko biến sẽ k tự động gán, con trỏ sẽ trỏ vào 1 nơi k xác định 
- nếu đã define move contructor thì bắt buộc phải define copy contructor

- có thể sử dụng constructors của lớp base bằng cách gọi nó ở vị trí member initializer lists, nếu không, khi khởi tạo thằng con, sẽ mặc định chỉ gọi default constructor của thằng base, nên dùng khi nào, khi cần khởi tạo các biến được delecare trong class cha, trách việc phải khởi tạo lại ở class con thôi !
class A{
public:
A(){}
A(int x){this->x = x;}

int x;
}

class B : public A{
public:
B(int x, int y) : this->y{y}, A(x){} -> dùng constructor của A để khởi tạo x

int y;
}

CHÚ Ý: con trỏ this là con trỏ ẩn trỏ vào đối tượng hiện tại, vậy nên trong các hàm thành viên có thể sử dụng con trỏ this
A a(0); sẽ hiểu như sau: a.A(0){a.this->x =0;}

- thành viên static sẽ không có con trỏ this, vì thành viên static thuộc về class, k thuộc về bất kì đối tượng nào !

- hàm bạn - function friend, class bạn - class friend, hàm thành viên là bạn

- Move constructor : đoạn này hơi ảo ảo ! 
CHÚ Ý: 1 cái vùng nhớ sẽ có 1 cái biến, hay là 1 cái expression đại diện cho vùng nhớ đó để chương trình có thể quản lý được cái vùng nhớ đó, tỷ như :
int global;
int main(){int n; return 0;}

thì ở đây có 2 cái biến (hoặc gọi là 2 cái expression) là global và n, mỗi cái đều dại điện cho 1 vùng nhớ kiểu int 4bytes, nhờ vào nó, chương trình sẽ biết được vòng đời tồn tại của vùng nhớ đó tương ứng với vòng đời tồn tại của biến !

vậy cái std::move là move vùng nhớ tới 1 cái name mới, giống kiểu ngôi nhà thay tên chủ hộ trên sổ đỏ vậy ! -> chủ cũ vô gia cư nhưng k bị bất hợp pháp(miễn là k undefine)

ví dụ đang có 1 class như thế này:
class A{
public:

    A(int x =0, int* p=nullptr) : mx{x}, ptr{p}{
        LOG<< "constructors: " << this;
    }
    A(const A& x){
        LOG << "copy constructors " << this << " " << &x;
    }
    A(const A&& x) noexcept {
        LOG << "move constructors " << this << " " << &x;
    }

    A& operator+(const A& x){
        LOG << "operator+ " << &x << " " << this;
        return *this;
    }
    A operator+(){
        LOG << "operator++";
        return *this;
    }
    void printed(){
        LOG << this;
    }
    ~A(){
        LOG << "destroyed " << this;
    }

public:
    const int mx = 0;
    int* ptr;
};
//A global;
A returnA(){
    A n;
    return A() hoặc return n đều như nhau
-> bởi vì return A() hay n đều là return về 1 giá trị của 1 object kiểu A, khi này chương trình sẽ tạo ra 1 cái định danh, hay gọi là 1 name tạm nào đấy quản lý cái vùng nhớ tạm chứa cái giá trị được return về !
-> và đây là return về 1 rvalue 
}

int main (){
    LOG << "main start !";
    A n = returnA();
    while(true){};
    return 0;
}

ta sẽ thấy lúc này chương trình nó đã sử dụng 1 cơ chế như là std::move vây ý, in log sẽ thấy, nếu ở main chỉ viết là return A()
, thì rõ ràng nó đã contructors ở function returnA và kết thúc hàm nó destroy cái đối tượng đó(mean destroy cái vùng nhớ đó), nhưng nếu viết như bên trên, A n = returnA(); -> vẫn sẽ là contructor 1 cái object từ function returnA này, nhưng sau đó k hề có copy contructor hay là move contructor để cretae ra object n nữa, ta cũng k hề thấy có object nào bị destroy đi, thế có nghĩa là, nó đã move cái vùng nhớ của object tạm được cretae ra ở function r trực tiếp sang cho thằng object n, hoặc hiểu là move cái định danh n đến cái vùng nhớ đó , và vòng đời của nó sẽ gán với n, nên ta sẽ k thấy có object nào bị destroy ở đây cả

-> TÓM LẠI: thấy, sau 1 hồi check, mình thấy move constructor hay copy contructor khác nhau mỗi chỗ là nó sẽ tiết kiệm quá trình gán giá trị, thay vì copy từ chỗ này sang chỗ khác, thì nó trỏ luôn tới đó ! ,refer lại cái cơ chế bên trên

- tưởng tượng như có 2 con trỏ, con trỏ 1 trỏ tới 1 mảng chưa có gì, con trỏ 2 trỏ tới 1 mảng nhiều phần từ, h nếu copy thì có nghĩa là copy từng phần tử từ mảng 2 -> 1, mất thời gian, move thì nó sẽ move luôn vùng nhớ của con trỏ 1 tới chỗ con trỏ 2, thành ra vẫn là 2 con trỏ trỏ cùng 1 vùng nhớ
-> sẽ dùng copy contructor nếu y =x mà gán xong rồi nhưng lại vẫn muốn dùng thằng x;

sẽ dùng move contructor nếu y = x và gán xong rồi k cần thằng x nữa, phải có step là gán x về null nếu trong x có con trỏ, nếu k sẽ bị 2 con trỏ trỏ cùng tới 1 vùng nhớ đó -> nói rồi, chỉ khác nhau ở hiệu suất chạy thôi

clean: nghĩa là áp dụng trong class thì hoàn toàn là do mình define nó ntn, move thì mình sẽ cho thằng mình đang tạo trỏ đến thằng kia, rồi cho thằng kia null, chứ nếu trong hàm move mà mình gán giá trị như bình thường thì chả khác 


- Operator Overloading in C++

ví dụ: sử dụng class A bên trên

int main (){

    A x(9), y(3);
    A z = x+y; // x.operator+(y), sau đó lại gọi copy contructors z.A('result after +'), nếu mà operator+ trả về là A& thì chính là z.A(x) ở đây x là kết quả sau khi thực hiện x+y
 
    return 0;
}

- Đặc biệt: overloading toán tử << và >> 
mình không thể viết kiểu này được:
    A operator << (type st){
        LOG << "operator++";
        return *this;
    }
-> A a; a << st tương đương với a.operator<<(st) -> HOÀN TOÀN SAI
vì sao: vì ở đây << nó là toán tử được overloading cho cout cin rồi, mặc định nó sẽ dùng để in được những kiểu dữ liệu có sẵn, bây giờ mình muốn in ra như thế nào thì mình overload lại,
nghĩa là mình overloading lại toán tử <<, >> của thằng cout cin cho nó có thể in ra cái đối tượng của mình

- NHÂN TIỆN: THIS POINTER
các hàm operator overload mà không có return cái gì để nó auto return giá trị rác về là oẳng !
A* <-> return this - trỏ 
A& <- return *this -> tham chiếu luôn
A <-> return *this -> giá trị thôi

ví dụ: ...
class A{
    public:
    int ma = 0;
    A* const foo1 (){
        ma = 1;
        return this;
    }
    A& foo2 (){
        ma = 2;
        return *this;
    }
    A* const foo3 (){
        ma = 3;
        return this;
    }
    void printed(){
        LOG << ma;
    }
};

A n;
n.foo1()->foo2().foo3()->printed(); // =3

- CONST OBJECT: Đối tượng hằng: ví dụ
class A{
private:
    int ma;
public:
    A(const int& a): ma{a} {}
    void getValue(){
        LOG << "function normal " << ma;

    }
    void getValue() const {
        LOG << "function const " << ma ;
    }
};

int main (){
    A a(9);
    a.getValue();

    const A b(3);
    b.getValue();
    while(true){}

    return 0;
}

- đối tượng const không thể gọi 1 hàm non-const dù hàm đó ko thay đổi giá trị của đối tượng
- đối tượng non-const có thể gọi cả hàm const và non-const
- hàm const thì ko thể thay đổi gía trị của object 
- khi tạo object của class con, nó sẽ luôn gọi default contructor của base

#include "logger.h"

class Base{
public:
    Base(){
        LOG << "constructor default base";
    }

    Base(int a) : ma{a}{
        LOG << "constructor base with one int";
    }

protected:
    int ma;
};

class Divered: public Base{
public:
    Divered(){
        LOG << "constructor default Divered";
    }

    // nếu không đưa Base{9} vào list khởi tạo, nó sẽ vẫn gọi default contructor của Base và do đó không thể khởi tạo ma cho đối tượng a của Divered
    Divered(int b) : Base{9},mb{b}{
        LOG << "constructor Divered with int";
    }
private:
    int mb;
};

int main(){
    Divered a(3);


    return 0;
}

- không thể khởi tạo trực tiếp biến thành viên không tĩnh của class base ở constructor của class con vì, ở 1 constructor thông thường k có ủy quyền, thứ tự khởi tạo sẽ là khởi tạo base, khởi tạo các member không tĩnh, chạy thân hàm -> base khởi tạo nó cũng khởi tạo member của nó -> con k thể khởi tạo lại, -> chỉ có thể dùng ủy quyền

class Base{
public:
correct:
explicit Base(int x) : mx{x} {}

protected:
int mx;
}

class Child: public Base{
public:
explicit Child(int x) : mx{x} {} -> WRONG WRONG WRONG

correct:
explicit Child(int x) : Base{x} {}, vậy thì bây giờ trên class phải có contructor khởi tạo với 1 biến x
}

==================================================================================================================================================================

9. STL Libsrary, std::function

FUNCTOR - function object là việc mình đi overload toán tử ()

template <typename T>
struct Chaelisa{
    T ma;
    void operator()(T x){
        QDEBUG << x*x << " ";
    }
};

int main(){

    Chaelisa<int> chli;
    chli.ma = 9;
    QDEBUG << chli.ma;
    chli(9); // chính là functor <-> chli.operator()(9) -> không phải là function

    return 0;
}

STL- https://www.geeksforgeeks.org/the-c-standard-template-library-stl/

1. generic programming with marcos

ví dụ nếu muốn viết so sanh 2 tham số với các kiểu tham số khác nhau như int, double... sẽ cần nhiều hàm khác nhau:

int max(int a, int b){
return a>b?a:b;
}
double max(double a, double b){
return a>b?a:b;
}

nhưng nếu dùng marco sẽ như sau:

#define MAX(a,b) (a>b?a:b) -> nó sẽ tự động nhận diện type của a, b -> processor
should 

khi gọi marco, sẽ sẽ gọi đúng những gì mình define, nên đừng tiếc dấu ngoặc đơn

2. generic programming with function templates

max function như trên có thể được viết bằng template function

template<typename T> // -> nói cho compiler biết đây là 1 template function và T là 1 template parameter
T max(T a, T b){
return a>b?a:b;
}

-> complier complier it ok, nhưng mà nó không gen ra bất kì code nào cả, nó chỉ đơn giản là 1 template, nó sẽ không gen ra code cho đến khi nào users chỉ định 1 version cụ thể cho template đó

- có thể sử dụng class thay cho typename -> template<class T>

int a,b;
first use: max<int>(a,b);
second use: just can max(a,b) cho dù type k phải là int nữa, nó vẫn sẽ ngầm hiểu
double c, d;
then just call max(c,d)

- CHÚ Ý: toán tử dùng trong hàm template phải được support bởi typename T
- có thể sừ dụng nhiều typename
template <typename T1, typename T2>
void func(T1 a, T2 b){
std::cout << a << b;
}

3. generic programming with class templates
same above !

4. creating a generic array template class

template<typename T, int N>
class A{
T size {N};
T array[N];
}

A<int, 10> a; -> template N, nó sẽ nhận giá trị n = 10, T là kiểu int

5. introduction to STL containers

- khi lưu trữ 1 object trong containers, nó sẽ make ra 1 bản sao của objetc chứ k đặt luôn cái object của mình vào trong container đâu (với all type luôn)

- các function hay dùng: 
size: return số lượng phần tử của container 
empty: return true or false nếu container empty or not
insert: chèn 1 phần tử tới container -> make a copy object của mình rồi chèn vào container
operator<, operator<=, operator>, operator>= : so sánh contents của 2 containers
operator==, operator!=: so sánh contents của 2 containers bằng nhau hay ko

swap: swap 2 phần tử của container
erase: xóa element(s) của containers
clear: xóa all element
begin and and: trả về trình lặp trỏ đến phần tử đầu hoặc cuối
rbegin and rend: trả về trình lặp ngược trỏ đến phần tử cuối hoặc đầu tiên
cbegin, cend: trình lặp hằng trỏ đến .....
crbegin, vrend: ..............

- CHÚ Ý: toán tử phải được hỗ trợ bởi object 

6. introduction to STL  Iterators

- nó như con trỏ để mà dùng ở trong container thôi
std::vector<int>::iterator it1; -> chỉ dùng trong vector int
std::list<std::string>::iterator it2; -> chỉ dùng trong list có phần tử là std::string
std::map<std::string, std::string>::iterator it3; -> chỉ dùng trong map có phần tử là std::string pair với st::string

ví dụ: std::vector<int> vec {1,2,3}; vec.begin là thằng 1, nhưng vec.end là thằng sau thằng 3 chứ k phải thằng 3

7. introduction to SLT algorithms

find(begin, end, element) // need to provide operator==
for_each()

8. std::array
9. std::vector
10. std::deque, std::queue, std::priority_queue
11. std::list
12. std::map
13. std::stack

-> https://en.cppreference.com/w/

File and Stream: https://www.geeksforgeeks.org/file-handling-c-classes/

==================================================================================================================================================================

10. Extern, liên kết ngoài - https://cafedev.vn/tu-hoc-c-c-lien-ket-ben-ngoai-trong-c/
Tóm tắt trước: mọi thứ nên check ở cpp, vì ở cpp include .h thì mới được tính có khai báo, file .h chỉ là để cho gọn gẽ để include vào trong .cpp thôi
liên kết là một thuộc tính của một định danh (không phải của một biến)

- định danh có 1 thuộc tính khác có tên là liên kết, có nghĩa là nó sẽ xác định xem các khai báo khác của tên đó có tham chiếu đến cùng 1 đối tượng hay không, kiểu là khai báo tên như thế nhưng có phải chiếu đến cùng 1 đối tượng, 1 vùng nhớ hay không hay không, nếu không thì sẽ có chuyện liên quan đến phạm vi của biến
- biến cục bộ không có liên kết, nghĩa là mỗi khai báo sẽ chỉ tham chiếu đến 1 đối tượng duy nhất

ví dụ:
int main()
{
    int x { 2 }; // local variable, no linkage
 
    {
        int x { 3 }; // this identifier x refers to a different object than the previous x
    }

    return 0;
}

- Phạm vi và liên kết: phạm vi thì xác định nơi có thể đọc biến đó và sử dụng 1 khai báo duy nhất, còn liên kết là xem xem có nhiều khai báo cùng tham chiếu đến 1 đối tượng hay không

- có 2 liên kết: là liên kết nội bộ và liên kết bên ngoài
- 1 biến non-const global sẽ có default là external linkage - liên kết ngoài
- nếu 1 biến global được khai báo là const, hoặc static, hoặc constexpr thì sẽ có default là internal linkage- liên kết trong

- về phạm vi của biến: trong 1 block, trong 1 file, trong 1 chương trình(gồm nhiều file) -> thì trong mỗi phạm vi đều phải tuân theo quy tắc 1 định nghĩa
-> các biến sẽ bị lỗi multi define or redefine nếu trùng tên trong cùng 1 phạm vi

- các object và function NỘI BỘ thì được xác định trong các file khác nhau là khác nhau, được coi là các thực thể độc lập, việc này thì cần nhìn xem nó có liên kết là internal linkage hay external linkage, nếu là external linkage thì nó sẽ được nhìn thấy ở nhiều file độc lập trong cùng 1 chương trình mà không cần include gì cả

ví dụ: 2 file a.cpp và b.cpp cùng trong 1 project
file a.cpp: static int g_x;
file b.cpp: static int g_x;
-> 2 biến g_x là độc lập vì chúng đều default là internal linkage

nhưng nếu :
file a.cpp:  int g_x;
file b.cpp:  int g_x; -> multi define, 
bởi vì default của global là external, nó có thể được nhìn thấy ở mọi file mà không cần include gì cả

- các function thì mặc định là liên kết ngoài, trừ khi dùng từ khóa static

- từ khóa extern sẽ tạo liên kết external cho định danh, nếu nó là external linkage rồi công dụng từ khóa này bị ignore đơn giản vì lúc này nó vô dụng

- Để dử dụng 1 biến toàn từ ở 1 file khác, cần 1 khai báo chuyển tiếp
- việc khai báo chuyển tiếp được thực hiện thông qua từ khóa extern(k có giá trị khở tạo)

ví dụ:
file a.cpp khai báo biến global là int g_x = 10;
file b.cpp muôn dùng nó thì cần extern int g_x; -> nói rằng đây là 1 khai báo chuyển tiếp cho thằng g_x đã tồn tại, nghĩa là thằng g_x này sẽ tham chiếu đến thằng g_x đã tồn tại trước đó ở đâu đó
-> include thì lại ăn redefine hay : hãy hiểu đơn giản lại về việc include là làm gì

- hàm thì không dùng từ khóa extern, nó sẽ hiểu là khai báo chuyển tiếp hay hàm mới thông qua việc mình có define thân hàm ko
- phạm vi file(được nhìn thấy trong 1 file- các biến toàn cục có liên kết là internal) < phạm vi toàn cầu(được nhìn thấy trong nhiều file, nghĩa là trong cả chương trình- các biến toàn cục có liên kết external)\

- lưu ý là 1 biến global trong file .h không ảnh hưởng đến file .cpp, vì khi chạy nó chạy file cpp mà, file .h chỉ để include

- nếu khai báo extern mà không khởi tạo, nó sẽ hiểu là khai báo chuyển tiếp cho 1 thằng đã được define ở đâu đó rồi, nếu mà thằng nào cũng extern rồi k khởi tạo, rõ ràng sẽ bị lỗi không tìm thấy thằng nào đã tồn tại để mà tham ch

==================================================================================================================================================================

11. Exception handing and Signal handing

try catch
try phải có catch, catch phải có try, đi cùng với nhau thành 1 cặp, cùng cấp
catch phải bắt đúng kiểu dữ liệu được throw ra, cat (...) thì mặc định là có thể bắt mọi loại kiểu dữ liệu
throw rồi thì nó sẽ k chạy tiếp những câu lệnh sau đó trong khối lệnh nữa vì tại đó nó hiểu là chương trình nó xảy ra 1 exception

signal handing: thì signal là các ngắt (interrup) buộc hệ điều hành phải thực thi tác vụ tương ứng, means kết thúc ngay cái tiến trình đang chạy

thì thằng C++ nó cũng cung cấp 1 số signal để mình có thể emit hoặc là bắt nó
thư viện cs
ví dụ: signal SIGSEGV được bắn ra khi mình truy cập vào vùng nhớ k hợp lệ(đa số sẽ làm chương trình crash)

SIGBUS: truy cập vào 1 địa chỉ không hợp lệ

==================================================================================================================================================================

12. Casting
static_cast<T>(expression): buộc chuyển đổi ngầm định, có thể cast non-const tới const, void* tới type*, pointer base* tới pointer derived*, pointer derived* tới base*
- static_cast đương nhiên sẽ check tính tương thích có thể cast khi complier nhưng không check tính an toàn hay toàn vẹn, ví dụ, cast từ 1 kiểu derived* về kiểu base* có thể gây mất dữ liệu chẳng h
- không thể cast từ const tới non-const -> chỉ có const_cast can do that
- nếu không phải kế thừa thì không thể cast theo kiểu con trỏ or reference mean T can not be a pointer or reference
- có thể cast 2 đối tượng chả liên quan gì tới nhau, thuộc 2 type khác nhau, chỗ này sẽ cần 1 contructor để cast - do cơ chế complier thôi, chịu, chắc nó kiểu wrap object
ví dụ:
class OneClass{
public:
    OneClass(){
        cout << "contructor OneClass" << endl;
    }
    void display() {
        cout << "one" << endl;
    }
    ~OneClass(){
        cout << "distructor OneClass" << endl;
    }
    int onex;
};
class TwoClass{
public:
    TwoClass(OneClass& params) : mparams{params} {
        cout << "contructor TwoClass" << endl;
    }
    void display() {
        cout << "two: " << " " << &mparams << endl;
    }
    ~TwoClass(){
        cout << "distructor TwoClass" << endl;
    }
    int twox;
    OneClass& mparams;
};
int main () {
    OneClass one;
    static_cast<TwoClass>(one).display(); it can
    return 0;
}
- chỗ này nó tạo 1 object temprary, mean cứ cast kiểu object thì nó sẽ tạo object temprary, nên mọi sự modify sẽ đươcj apply trên object temp đó chứ k phải trên current object
=> dùng kiểu static_cast<type>(*this)->function() -> nó sẽ tạo ra tem object và thằng function sẽ được gọi trên tem object -> mọi modify sẽ trên tem object chứ k phải trên current object
/*----------------------------------------------------------------------------------*/
dynamic_cast<T>(expression) : chỉ chấp nhận cast kiểu con trỏ or reference, cast trong runtime, thực sự tốn kém vì nó so sánh và cast đến kiểu thực sự của object, nếu cast k thành công thì null
ví dụ:
class Animal{
public:
    virtual void func() {}
};
class Dog : public Animal{
    public:
    void makesound_dog(){}
}
class Cat : public Animal{
    public:
    void makesound_cat(){}
}
int main () {
    Animal* dog = new Dog;
    if(Dog* ds = dynamic_cast<Dog*>(dog)){ // Cat* cat = dynamic_cast<Cat*>(dog) // cast thành công thì trả về object, k thành công trả về null
        cout << "ok" << endl;
    }
    else{
        cout << "fail" << endl;
    }
    // hi vọng thằng Animal* dog có thể cast về kiểu Cat* để sau đó gọi makesound_cat, nhưng rõ ràng ko được -> gây crash
    dynamic_cast<Cat*>(dog).makesound_cat(); // cast k thành công trả về null mà dùng thì hiển nhiên là gây crash rồi
    // nếu thích mạo hiểm, dùng try catch sẽ k sao
    try{
        dynamic_cast<Cat*>(dog).makesound_cat();
    }catch(...){}
    return 0;
}
- phải có virtual thì mới cho dùng dynamic_cast
- không được phép sử dụng một biến tham chiếu (hoặc con trỏ) là kết quả của việc ép kiểu mà không chắc chắn rằng việc ép kiểu đó thành công.

==================================================================================================================================================================

13. lambda

- A closure class is a class from which a closure is instantiated. Each lambda causes
compilers to generate a unique closure class. The statements inside a lambda
become executable instructions in the member functions of its closure class.

- A closure is the runtime object created by a lambda. Depending on the capture
mode, closures hold copies of or references to the captured data

- closure class -> class ẩn được complier gen ra mà mình chả rõ chính xác type của nó là gì, mỗi lamdba sẽ làm cho complier gen ra 1 unique closure class

* ví dụ
class Widget {
...
}
- thì nó tường mình và mọi instance đều có type là Widget, nhưng closure class thì chả biết type của instance của nó là gì -> dùng auto thôi

- thì thằng lambda bản chất nó là nó trả về 1 instance của closure class, do đó phải dùng biến auto đấy, có nghĩa nó là 1 object đấy

*** kiểu
Widget returnWidget(){
    return Widget;
}
Widget wd = returnWidget();


closure_class lambda(){
    return closure_class;
}

auto lambda = [](){}; mean [](){} là 1 biểu thức lambda nó sẽ trả về 1 instance của 1 closure_class mà gen ra từ complier 

[](){}()
[]: capture
(): tham số
{}: thân hàm của lambda
(): truyền giá trị cho tham số và run lambda

-> nó tương tự như khai báo lambda bên trên và call nó chạy lambda();

- và cái việc chạy [](){}() hay là lambda() nó tương tự như kiểu chạy function object của class thôi 

*** ví dụ :
    int lambda = []() ->  int{
        static int n = 1;
        n++;
        cout << "lambda: " << n << endl;
        return n;
    };
    
nó sẽ kiểu kiểu như:    
    class closure_class {
        public :
            int operator () {
                static int n = 1;
                n++;
                cout << "lambda: " << n << endl;
                return n;
            }
    }
- type return về và tham số truyền vào thì tùy vào lambda như thế nào

*** về capture: có 2 kiểu là capture by value và capture by reference
việc mình capture làm cho cái closure sẽ gen ra variable tương ứng trong scope nơi lambda được define, nếu capture by value gen 1 biến value bình thường và gán cho giá trị "bằng" thôi,
còn capture by reference thì nó tạo biến reference tới cái biến nó capture
-> nếu mà lifetime của closure created from lambda tồn tại lâu hơn cái biến được capture -> cái biến reference trong closure tồn tại lâu hơn -> trạng thái dangle

ví dụ:
using FilterContainer = std::vector<std::function<bool(int)>>; có 1 cái vector chứa các con trỏ hàm
FilterContainer filters;

void addDivisorFilter()
{
    auto calc1 = computeSomeValue1();
    auto calc2 = computeSomeValue2();
    auto divisor = computeDivisor(calc1, calc2);
    filters.emplace_back( // danger!
                          [&](int value) { return value % divisor == 0; } // ref to
    ); // divisor
}

- divisor là 1 biến thuộc scope funtion addDivisorFilter, nó sẽ bị hủy, nó sẽ k còn tồn tại khi hàm này kết thúc
- filters là 1 biến global, bây giờ add cái lambda đó làm 1 phần tử trong vector, thì mỗi lần gọi phần tử đó ra, cái biến ref trong lambda refer to divisor
mà thằng divisor bị hủy cụ rồi -> dangle -> undefine behavior

ví dụ khác:
using FilterContainer = std::vector<int*>;
FilterContainer filters;

void addDivisorFilter()
{
    int n = 9;
    cout << &n << endl;
    filters.push_back( [&](){
        return &n;
    }());
}

int main (){
    cout << "start" << endl;
    addDivisorFilter();
    int n = 9;
    cout << filters[0] << " " << *(filters[0]) << endl;

    return 0;
}

-> nếu mà cái vùng nhớ của cái biến trong funtion bị hủy đi r, bị sử dụng rồi, thì sẽ oẳng, còn nó vẫn còn free thì nó vẫn chọc vào được và vẫn chạy thôi => thế mới gọi là Undefine Behavior

-> trong TH này thì chỉ captured by value thôi
- capture by value cũng có thể dangle khi value lại là con trỏ 

- chú ý lambda trong function member của class, capture kiểu [=] hoặc là [&] đều là capture con trỏ this thôi

*** OKE, Now read here: Use init capture to move objects into closures.
- đã hiểu closure rồi thì hiểu move object tới closure nó cũng ảo ấy chứ
nhắc lại: bản chất của lambda là nó return về 1 instance của closure classs và nó chạy như 1 functor luôn, thế nên cái đoạn int capture nó như mình khởi tạo biến thành viên
của cái class closure đó thôi

Using an init capture makes it possible for you to specify
1. the name of a data member in the closure class generated from the lambda and
2. an expression initializing that data member.

class Widget;
auto pw = std::make_unique<Widget>(); // create Widget;
auto func = [pw = std::move(pw)] // init data mbr
 { return pw->isValidated() // pw dùng ở đây là pw của closure class
 && pw->isArchived(); };

- bên trái dấu "=" là tên của data member trong closure class, bên phải dấu "=" là biểu thức khởi tạo , scope trái phải là khác nhau
scope bên trái là của closure class - nó tương đương với scope của thằng auto lambda kia, scope bên phải là của thằng pw local bên trên
pw bên trái refer to data member trong closure class, pw bên phải là cái biến bên trên lambda đó

So “pw = std::move(pw)” means “create
a data member pw in the closure, and initialize that data member with the result of
applying std::move to the local variable pw.”

làm rõ 
auto func = [pw = std::move(pw)] // init data mbr
 { return pw->isValidated() // pw dùng ở đây là pw của closure class
 && pw->isArchived(); };
-> khai báo lambda, lúc này lambda chưa hề chạy, nó trả về 1 instance của closure class và gán và auto -> khi nào auto bị hủy thì 
thằng pw bên trong mới bị hủy
 và

 [pw = std::move(pw)] // init data mbr
 { return pw->isValidated() && pw->isArchived(); }();
 -> còn ở đây nó chạy mẹ luôn rồi, nó trả về instance của closure class nhưng k gán cho 1 thằng nào để extend vòng đời của nó nên nó bị 
 hủy luôn ngay sau khi gọi