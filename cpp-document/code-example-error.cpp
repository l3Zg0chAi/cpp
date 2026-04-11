=============================================================================================================================================================

class base {
public:
    virtual void funcBase() {}
};

class derived : public base {
public:
    virtual void funcBase() {}
    virtual void funcDerived() {} // I add virtual keyword to make sure funcDerived method is added in vtable of class
};

int main()
{
    base* obj = new derived;
    obj->funcBase(); // will print funcBase in class derived -> it's normal

    obj->funcDerived(); // error: 'class base' has no member named 'funcDerived'
    return 0;
}

--> nhìn lại vào dòng code : base* obj = new derived;
- tại thời điểm biên dịch, thì complier sẽ kiểm tra xem class derived có kế thừa class base hay không,
                          (và nó hoàn toàn biết là có, khi đó thì nó biết 2 thứ, static type là base*, còn dynamic type là derived* thì phải trong runtime)

Nếu có, thì con trỏ "obj" được tạo ra là kiểu "base*" và được gán trỏ tới một đối tượng của lớp "derived" là hợp lệ, tại đây sẽ không có khởi tạo vùng nhớ
cho object bởi new, do đó trình biên dịch chưa thể biết chính xác kiểu của đối tượng (chuẩn, vì lúc này nó chỉ biết static type là base* thôi), 
chỉ đến khi chương trình chạy, vùng nhớ được khởi tạo, con trỏ trỏ tới, lúc đó mới biết là chính xác đối tượng đó là kiểu derived*

-> vậy nên lỗi là tại thời điểm complie,complier không biết được rằng obj có trỏ đến 1 object kiểu derived thật không hay chỉ là 1 kiểu của class khác cũng 
kế thừa base, thì class đó nó đâu có thằng funcDerived -> gọi funcDerived là hoàn toàn k hợp lý

tóm lại là:
Compiler chỉ cho phép gọi những member có trong static type của biểu thức: ở đây static type là base*, nên chỉ gọi được những hàm mà base khai báo.
Virtual chỉ có tác dụng: sau khi compiler đã xác nhận lời gọi là hợp lệ thì runtime mới quyết định gọi implementation nào
Nó không làm cho base* tự động thấy được hàm mới của derived.

cách đơn giản (case là được)
=============================================================================================================================================================

#include <iostream>

void myFunction(int&& num) {
    std::cout << "The value of num is: " << num << std::endl;
}

int main() {
    int x = 4;
    int && r = 1998;
    myFunction(std::move(x)); // cast lvalue to rvalue reference using std::move()
    myFunction(12); // pass an rvalue argument directly
    myFunction(r) // -> error
    myFunction(std::move(r));
    return 0;
}

- hàm có tham số là rvalue ref có thể nhận rvalue hoặc rvalue ref, thằng std::move sẽ trả về rvalue reference 
- ở trên khai báo rõ ràng r là 1 rvalue ref, nhưng lúc truyền vào hàm lại error, bởi vì đù cmn do cơ chế thằng C++ nó coi bất kể thằng nào có 1 cái tên, hay gọi 
là 1 cái định danh đi, thì khi truyền vào hàm đều là lvalue, nên lúc đó mới phải dùng std::move

=============================================================================================================================================================