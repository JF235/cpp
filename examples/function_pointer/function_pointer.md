# Syntax For Using Function Pointers In C++

```cpp
int add(int a, int b) {
    return a + b;
}

int main(void) {
    int (*func_ptr)(int, int) = add;
    int result = func_ptr(2, 3);
    return 0;
}
```

# Alias For Function Pointers

```cpp
int add(int a, int b) {
    return a + b;
}

int main(void) {
    using func_ptr = int (*)(int, int);
    func_ptr ptr = add;
    int result = ptr(2, 3);
    return 0;
}
```