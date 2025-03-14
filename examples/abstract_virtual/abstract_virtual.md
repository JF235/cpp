# Herança

Quando uma classe (chamada de classe derivada ou subclasse) herda atributos e métodos de uma outra classe (chamada de classe base ou superclasse). A sintaxe nesses casos é

```cpp
class mySubclass : public mySuperclass {
//...
};
```

Isso faz com que a classe base herde de forma *public*.

Nos casos de herança *private*, tudo da classe base é convertido para private (campos public e protected).

Quando a palavra é omitida, ela será private para declarações com a palavra class e public, no caso da palavra chave struct (ver [Public vs Private](#public-vs-private)).

## Public vs Private

Os métodos e atributos que são utilizados pelo programa, fora da própria classe, devem ser precedidos pela palavra chave public.

- Para classes definidas com a palavra chave **class** o acesso padrão é *private*.
- Para classes definidas com a palavra chave **struct** o acesso padrão é *public*.

```cpp
class myClass {
public:
// Tudo que é usado fora da classe vem aqui

private:
// Tudo que não é usado fica aqui

};
```

Quando eu uso um método private fora da classe, recebo um erro de compilação

```terminal
error: '...' is private within this context
```

## Virtual

Em c++, classes abstratas apresentam métodos com a seguinte assinatura

```cpp
virtual size_t get_data_size() = 0;
```

A palavra virtual significa que uma classe derivada (subclasse) pode sobrescrever a implementação da classe original. Isso é feito usando a palavra chave **override** (que apesar de altamente recomendade, não é obrigatória).

Quando a palavra virtual é seguida de `=0`, trata-se de um *método virtual puro*. Isso significa que as subclasses devem obrigatoriamente implementar uma versão desse método.

Em C++, classes abstratas podem implementar alguns métodos. Não há maneiras de forçar que uma classe abstrata seja somente uma interface (somente métodos virtuais puros).