# Criação e Destruição Automática

- Construtores: métodos que tem o mesmo nome da classe e são chamados quando um objeto é criado.
- Destrutores: métodos que tem o mesmo nome da classe, precedido por `~`, e são chamados quando um objeto é destruído.

Note que ambos os métodos são públicos, por padrão pois criamos a classe com a palavra-chave `struct`. Se a classe fosse criada com a palavra-chave `class`, os métodos seriam privados por padrão e dariam erro ao tentar compilar.

```cpp
#include <iostream>

struct myClass {
    myClass() {
        std::cout << "Creating myClass::myClass()" << std::endl;
    }

    ~myClass() {
        std::cout << "Destroying myClass::~myClass()" << std::endl;
    }
};

int main(){
    myClass obj;
    return 0;
    /* Output
    Creating myClass::myClass()
    Destroying myClass::~myClass()
    */
}
```

# Múltiplos Construtores

```cpp
struct myClass {
    int number = 0;

    myClass() {
        std::cout << "Creating myClass::myClass() with " << number << std::endl;
    }

    myClass(int n) {
        number = n;
        std::cout << "Creating myClass::myClass() with " << number << std::endl;
    }

    // ...
};

int main(){
    myClass obj;
    myClass obj2(10);
    return 0;
}
```

# Construtores e Destrutores padrão

Os construtores e destrutores são criados automaticamente pelo compilador se não forem definidos. O comportamento padrão é:

- Construtor padrão: inicializa todos os membros da classe com valores padrão.
- Destrutor padrão: não faz nada.

# `new` e `delete` keywords

O operador `new`:
- Aloca memória dinamicamente no heap (em vez da pilha/stack)
- Chama o construtor do objeto
- Retorna um ponteiro para o objeto criado

```cpp
MyClass* objeto = new MyClass();       // Com construtor padrão
MyClass* objeto = new MyClass(param);  // Com construtor parametrizado
int* array = new int[10];              // Aloca array dinâmico
```

O operador `delete`:
- Chama o destrutor do objeto
- Libera a memória alocada dinamicamente
- Evita vazamentos de memória (memory leaks)

```cpp
delete objeto;     // Para objetos únicos
delete[] array;    // Para arrays alocados com new[]
```

## Quando usar...

Use `new` quando:
- Precisa que um objeto exista além do escopo de criação
- Não sabe o tamanho necessário em tempo de compilação
- Precisa criar muitos objetos cujo tamanho total excederia a pilha

```cpp
void criaObjeto() {
    MyClass* objeto = new MyClass();
    // objeto vai continuar existindo após a função terminar
    // mas precisará ser deletado manualmente depois
    outroMetodo(objeto);
}
```

## Cuidados...

- **Vazamentos de memória**: Se usar `new` sem o respectivo `delete`
- **Ponteiros pendentes**: Acessar memória após `delete` (dangling pointers)
- **Exceções**: Se o construtor lançar uma exceção após `new`

## 5. Alternativas modernas (C++11 e posteriores)

Os ponteiros inteligentes são alternativas mais seguras:
```cpp
#include <memory>

std::unique_ptr<MyClass> obj = std::make_unique<MyClass>(); // C++14
std::shared_ptr<MyClass> objCompartilhado = std::make_shared<MyClass>();
```

Eles gerenciam a memória automaticamente e chamam `delete` quando apropriado.

## Exemplo

```cpp
#include <iostream>

class Recurso {
public:
    Recurso() { std::cout << "Alocando recurso\n"; }
    ~Recurso() { std::cout << "Liberando recurso\n"; }
    void usar() { std::cout << "Usando recurso\n"; }
};

int main() {
    // Alocação dinâmica
    Recurso* r = new Recurso();
    r->usar();
    delete r;  // Importante para evitar vazamento de memória
    
    // Versão com alocação automática (stack)
    {
        Recurso r2;  // Não usa new/delete
        r2.usar();
    }  // r2 é destruído automaticamente aqui
    
    return 0;
}
```

# Valores Padrão

Existem algumas maneiras de inicializar valores padrão para os membros da classe.
1. Valores padrão no corpo da classe.
    ```cpp
    struct myClass {
        int number = 0;
    };
    ```
2. Lista de inicialização, que é uma lista de pares `nome_do_atributo(valor)`. A lista de inicialização é colocada entre `:` e `{}`.
    ```cpp
    struct myClass {
        int number;

        myClass() : number(0) {}
    };
    ```

# Atribuição Inicial

Existem algumas maneiras de inicializar valores para os membros da classe.
1. Atribuição.
    ```cpp
    struct myClass {
        int number;

        myClass(int n) {
            number = n;
        }
    };
    ```
2. Lista de inicialização.
    ```cpp
    struct myClass {
        int number;

        myClass(int n) : number(n) {}
    };
    ```

As vantagens da lista de inicialização são:
- Atribuição de valores constantes.
- Atribuição de valores de referência.

Um exemplo mais elaborado de construtor com lista de inicialização é:    

```cpp
#include <iostream>
#include <string>

class Pessoa {
private:
    int idade;
    std::string nome;
    const int id;
    double altura;

public:
    Pessoa(int i, std::string n, int identificador, double alt = 1.75)
        : idade(i), nome(n), id(identificador), altura(alt) {
        std::cout << "Pessoa criada: " << nome << std::endl;
    }
    
    void mostrarInfo() const {
        std::cout << "Nome: " << nome << ", Idade: " << idade 
                  << ", ID: " << id << ", Altura: " << altura << std::endl;
    }
};

int main() {
    Pessoa p1(25, "João", 1001);
    Pessoa p2(30, "Maria", 1002, 1.65);
    
    p1.mostrarInfo();
    p2.mostrarInfo();
    
    return 0;
}
```

# Particularidades de Destrutores

Destrutores são precedidos por `~` e não possuem parâmetros. Uma classe chamada de `MyClass` tem o destrutor definido como

```cpp
~MyClass() {
    // ...
}
```

O destrutor é chamado automaticamente quando um objeto é destruído. As maneiras de destruir um objeto são:
1. Quando o objeto sai do escopo em que foi criado.
    ```cpp
    int main(){
        {
            myClass obj;
        }
        return 0;
    }
    ```
2. Quando um ponteiro para o objeto é deletado.
    ```cpp
    int main(){
        myClass* obj = new myClass();
        delete obj;
        return 0;
    }
    ```
3. Quando um objeto é criado em um ponteiro inteligente e o ponteiro inteligente é destruído.
    ```cpp
    #include <memory>

    int main(){
        std::unique_ptr<myClass> obj = std::make_unique<myClass>();
        return 0;
    }
    ```

Dessa maneira, o método destrutor precisa ser público para que o objeto possa ser destruído. O mesmo vale para o construtor.