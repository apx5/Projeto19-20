% Titulo
% André Sá a76361; João Rodrigues a84505; Pedro Oliveira a86328
% 2020-07-21

## Introducao

O paradigma funcional tem ganho notorieadade junto de grandes empresas e
programadores em detrimento de outros, pois permite que em poucas linhas de
código, quando comparado com outros estilos, se consiga criar soluções robustas
e eficientes.

Neste documento será dada ênfase às vantagens do paradigma funcional e de que
forma podemos aproveitar essas vantagens em `C++`.

Iremos estudar e analisar as características funcionais em programas escritos
em `C++`, através de algumas bibliotecas existentes para esse efeito e,
aproveitaremos para efectuar uma análise comparativa de performance, sintaxe,
etc, através de programas que resolvem o mesmo problema em `C++` e `Haskell`.

O uso de `template`s em `C++` traz algumas vantagens à programação em estilo
funcional, nomeadamente a possibilidade de fazer programação genérica, isto é,
criar programas polimórficos. Também é possível obter computação em tempo de
compilação com `template`s, mas esta não é essencial a Programação Funcional, e
portanto não vamos desenvolver sobre este assunto.[^let_over_lambda]

Aproveitaremos também para aprofundar alguns aspectos/características importantes da
Programação Funcional tais como:

 * Imutabilidade
 * Lazy Evaluation
 * Composição
 * ADTs

Quando necessário, e para uma melhor elucidação sobre as questões que estão a
ser analisadas, serão usados pequenos excertos de código em ambas as
linguagens.


## Abordagem ao Paradigma Funcional em `Haskell` e `C++`

### O paradigma funcional

Na programação funcional, os programas são executados através da avaliação de
expressões, em contraste, por exemplo, com o paradigma imperativo onde os
programas são compostos por instruções que vão alterando o estado global à
medida que executam. Isto significa que os procedimentos podem ter acesso ao
estado global e/ou partilhado entre varios procedimentos. Esta partilha não
está especificada de forma nenhuma e, portanto, tem de ser o programador a
cuidar e evitar que problemas aconteçam. O paradigma Funcional evita este
problema parcial ou completamente, ao desencorajar ou impedir esta prática e,
ao mesmo tempo, encorajar e facilitar "boa pratica".

Um exemplo extremo e pouco realista seria:

```cpp
void accoes (void)
{
    accao1();
    accao2();
    accao3();
}
```

Deste pequeno excerto, podemos concluír uma de duas hipóteses:

 1. Como nenhum dos procedimentos `accao1`, `accao2` ou `accao3` recebe
    argumentos, e o resultado não é utilizado, então estes procedimentos não
    fazem nada de útil e, portanto, `accoes` também não faz nada de útil;

 2. Cada um dos procedimentos faz algo de útil, mas para tal acede e altera
    alguma estrutura de dados partilhada; esta relacao _input_-_output_ não é
    explicita.

Por outro lado, numa linguagem funcional escreveriamos (em notacao `Haskell`)
`accoes = accao3 . accao2 . accao1`{.hs} para representar a mesma sequência de
acções mas sem partilha de memória nem estruturas de dados a serem mutadas:
cada uma das acções é uma função que devolve uma estrutura de dados, dada outra
estrutura de dados.

Este problema de alteração implicita de estado agrava-se ainda mais num
contexto concorrente com threads e partilha de memoria.

### `Haskell` como linguagem funcionalmente pura

`Haskell` adopta o paradigma Funcionalmente Puro, o que quer dizer que um
programa é uma funcao no sentido matemático, ou seja, dado o mesmo _input_ é
sempre devolvido o mesmo _output_.

Para se implementar efeitos secundários, em `Haskell`, em vez de se aceder ao
mundo e alterá-lo implicitamente, como na maioria das linguagens, este é
recebido como um argumento, e as mudanças são feitas sobre esse argumento.

Para dar melhor a entender, vejamos um exemplo: `puts`{.c}. O seu protótipo em
`C` é `int puts (const char *s)`{.c}. A string parâmetro `s` vai ser impressa
no `stdout`, mas nada no tipo da função nos diz que assim é.

Em `Haskell`, a função equivalente é `putStrLn`, com tipo `String -> IO
()`{.hs}, e o efeito secundário de imprimir a string de _input_ no `stdout`
está descrito no próprio tipo da função: `IO ()`.

Pode-se pensar neste `IO a` como sendo `World -> (a, World)`, ou seja, dado um
mundo, é devolvido o resultado da computação, e o novo mundo.[^awkward_squad]

### `C++ "funcional"`

Devido à sua herança, `C++` promove um estilo frágil de programação, devendo
ser o programador a ter alguma atenção e a tomar algumas decisões quando
pretende usar o paradigma funcional em `C++`. Por exemplo:

 * Evitar dados mutáveis. Numa função que altera uma estrutura, em vez de
   receber a estrutura por referência e a alterar, será melhor receber a
   estrutura por valor e devolver uma nova. Por razões de performance, tambem
   pode ser boa ideia passar a estrutura por referencia `const`{.c}, que se
   traduz em menos movimentação de memória.

 * Para um estilo de programação mais genérico, mas ao mesmo tempo mais seguro,
   preferir `template`s a `void *`, o que permite uma abstração de tipos, indo
   de encontro ao que acontece em `Haskell`. Vejamos o exemplo de uma função
   que soma dois valores passados como argumento.

   ```cpp
   template <typename T>
   T add(T a, T b) {
      return a + b;
   };

   int main ()
   {
      auto i = add(2, 3);
      auto f = add(2.2, 4.1);
      return 0;
   }
   ```

   Esta função pode ser invocada com diferentes tipos, tornando desnecessária a
   implementação da mesma função para tipos diferentes, e ganhando de forma
   gratuita a inferência de tipos por parte do compilador, através da keyword
   `auto`.

 * Recorrer ao uso de _lambdas_ para criar abstrações (desde `C++11`)
 * Utilizar bibliotecas funcionais existentes (_Fplus_, _Cpp prelude_, _Ranges_)

### Comparação e análise de programas equivalentes em `Haskell` e `C++`

Neste capítulo, faremos uma comparação mais específica sobre programas escritos
em âmbas as linguagens e cujo propósito é o mesmo, ou seja, podem considerar-se
equivalentes. Durante a pesquisa que efectuamos, encontramos duas bibliotecas
que tentam transpôr o paradigma funcional para `C++`, que vão de encontro aos
objectivos do nosso projeto. Vamos começar por algumas funções sobre listas do
_prelude_ do `Haskell`, usando a biblioteca _"CPP Prelude"_[^cpp_prelude], para
uma comparação mais directa, e terminaremos com um programa mais robusto que
foi utilizado na ronda de qualificação do _Google Hash Code 2020_, do qual
tinhamos a versão em `Haskell` e fizemos a conversão para `C++` utilizando a
biblioteca _"Functional Plus"_[^fplus], para uma comparação mais realista.

#### _Prelude_

De forma a comparar a performance de pequenos programas em âmbas as linguagens, geramos um ficheiro de _input_ com uma 
lista de 10000000 deinteiros. Note-se que deixamos de fora da análise o processo de leitura do
ficheiro. Focaremos a comparação na aplicação de funções específicas em
`Haskell` e `C++`. Para cada função, vamos apresentar uma definição com recursividade explícita
e uma definição recorrendo a funções de ordem superior(em Haskell), seguidas de uma implementação
em C++ e no final apresentamos os tempos de execução.

##### `map`

Comecemos pelo `map`{.hs}. Esta função _mapeia_ todos os elementos de uma dada
lista com uma dada função. Por exemplo, em `Haskell`, se tivermos uma lista de
inteiros `l :: [Int]`{.hs} e quisermos duplicar todos os elements da lista,
basta chamar `map (*2) l`{.hs}.

Em `Haskell`:

```hs
map :: (a -> b) -> [a] -> [b]

-- Recursividade explícita
map f [] = []
map f (h:t) = f h : map f t

-- Funções de ordem superior
map f = foldr (\a b -> f a : b) []
```

Em `C++`:

```cpp
template <Function FN, Container CN, Type A,
          Type B = typename std::result_of<FN(A)>::type,
          typename AllocA = std::allocator<A>,
          typename AllocB = std::allocator<B>>
auto map(const FN& f, const CN<A, AllocA>& c) -> CN<B, AllocB> {
  auto res = CN<B, AllocB>{};
  res.reserve(c.size());
  std::transform(std::begin(c), std::end(c), std::back_inserter(res), f);
  return res;
}
```

Em `C++` já existe uma função parecida: `std::transform`{.cpp}. Esta função
recebe os iteradores de início e fim da colecção de _input_, a forma como se
deve inserir na colecção de resultado, e como transformar cada elemento da
colecção de _input_; e devolve o iterador para o início da colecção de
resultado.

Como tal, podemos aproveitar o `std::transform`{.cpp} para definir o `map`{.hs}
em `C++`.  Como o `map`{.hs} devolve uma colecção, temos de criar uma a
colecção de resultado (`res`{.cpp}) -- em `Haskell` isto é feito de forma
automática.

##### `filter`

A segunda função que comparamos foi o `filter`{.hs}, que recebe uma lista e um
predicado, e calcula a lista que tem todos os elementos que satisfazem esse
predicado. Por exemplo, se tivermos uma lista de inteiros `l :: [Int]`{.hs}, e
quisermos obter a lista dos inteiros pares, podemos usar o `filter`{.hs} com o
predicado `even`{.hs}: `filter even l`{.hs}.

Em `Haskell`:

```hs
filter :: (a -> Bool) -> [a] -> [a]

-- Recursividade explícita
filter p [] = []
filter p (h:t)
    | p h       = h : filter p t
    | otherwise =     filter p t

-- Funções de ordem superior
filter p = foldr (\a b -> if p a then a:b else b) []
```

Em `C++`:

```cpp
template <Predicate PR, Container CN, Type A,
          typename AllocA = std::allocator<A>>
auto filter(const PR& p, const CN<A, AllocA>& c) -> CN<A, AllocA> {
  auto res = CN<A, AllocA>{};
  res.reserve(c.size());
  std::copy_if(std::begin(c), std::end(c), std::back_inserter(res), p);
  res.shrink_to_fit();
  return res;
}
```

Tal como no caso do `map`{.hs}, já existe uma função parecida:
`std::copy_if`{.cpp}.  Apesar de não sabermos à partida quantos elementos terá
a colecção de resultado, por razões de performance, podemos na mesma reservar
espaço. No fim, a colecção pode conter menos elementos que os reservados, e
para remover a memória inutilizada, usa-se `shrink_to_fit`{.cpp}.

##### `reverse`

A nossa terceira função escolhida foi o `reverse`{.hs} que, dada uma lista,
inverte a ordem dos seus elementos. Por exemplo, se tivermos a lista `l = [1,
2, 3, 4, 5]`{.hs}, e lhe aplicarmos o `reverse`{.hs} obtemos `[5, 4, 3, 2,
1]`{.hs}.

Em `Haskell`:

```hs
reverse :: [a] -> [a]

-- Recursividade explícita
reverse = reverse' []
    where
        reverse' ret []    = ret
        reverse' ret (h:t) = reverse' (h:ret) t

-- Funções de ordem superior
reverse = foldl (flip (:)) []
```

Em `C++`:

```cpp
template <Container CN, Type A, typename AllocA = std::allocator<A>>
auto reverse(const CN<A, AllocA>& c) -> CN<A, AllocA> {
  auto res = CN<A, AllocA>{c};
  std::reverse(std::begin(res), std::end(res));
  return res;
}
```

Mais uma vez, já existe uma função parecida: `std::reverse`. No entanto, o
`std::reverse` altera a colecção, em vez de devolver uma nova.

##### `zip`

Para concluir o primeiro conjunto de funções escolhemos a função `zip`. Esta
recebe duas listas, e emparelha os seus elementos -- o primeiro com o primeiro,
o segundo com o segundo, etc. Caso as listas tenham tamanhos diferentes a menor
lista dita o tamanho final.

Em `Haskell`:

```hs
zip :: [a] -> [b] -> [(a,b)]

-- Recursividade explícita
zip []     _      = []
zip _      []     = []
zip (x:xs) (y:ys) = (x, y) : zip xs ys

-- Funções de ordem superior
zip = zipWith (,)
```

Em `C++`:

```cpp
template <Container CA, Type A, typename AllocA = std::allocator<A>,
          Container CB, Type B, typename AllocB = std::allocator<B>,
          Container CRES = CA, typename RES = std::tuple<A, B>,
          typename AllocRES = std::allocator<RES>>
auto zip(const CA<A, AllocA>& left, const CB<B, AllocB>& right)
    -> CRES<RES, AllocRES> {
  auto res = CRES<RES, AllocRES>{};
  res.reserve((left.size() < right.size()) ? left.size() : right.size());
  auto l = std::begin(left);
  auto r = std::begin(right);
  while (l != std::end(left) && r != std::end(right)) {
    res.emplace_back(*l, *r);
    ++l;
    ++r;
  }
  return res;
}
```

Neste caso, não existe nenhuma função parecida na STL, e portanto, é definida manualmente como um ciclo.

##### Resultados

Para comparar performance entre as duas linguagens, medimos o tempo de CPU de
cada função, com os meios disponíveis em cada uma. Simultaneamente medimos o
tempo de execução real do processo com o programa `/usr/bin/time`.

Em `C++` usamos `std::clock()` do _header_ `<ctime>`, com o seguite macro:

```cpp
#define benchmark(str, func) \
  do { \
    auto start = std::clock();                 \
    (void) func;                               \
    auto stop = std::clock();                  \
    auto duration = 1000000000                 \
                  * (stop - start)             \
                  / CLOCKS_PER_SEC;            \
    std::cout << str << ": " << duration       \
              << " nanoseconds" <<  std::endl; \
  } while (0)
```

Em `Haskell` usamos `getCPUTime` de `System.CPUTime`, com a seguinte função:

```hs
timeSomething :: NFData a => String -> a -> IO ()
timeSomething str something = do
  start <- liftIO getCPUTime
  let !result = deepforce $! something
  end <- liftIO getCPUTime
  let diff = round . (/1000) . fromIntegral $ end - start
  putStrLn $ str ++ ": " ++ show diff ++ " nanoseconds"
```

NOTA: Como `Haskell` é _lazy by default_, para obter-mos uma comparação justa é necessário forçar a avaliação das expressões que 
pretendemos testar. Para isso usamos o `deepforce`, que está definido como `deepforce x = deepseq x x`{.hs}, sendo `deepseq a b` a função que força
a avaliação de `a` e devolve `b`.


|                             | `C++`   | `Haskell` |
| :-------------------------: | :-----: | :-------: |
| `map (*2)`                  | 14 ms   | 149 ms    |
| `filter even`               | 48 ms   | 139 ms    |
| `reverse`                   | 11 ms   | 806 ms    |
| `uncurry zip . split id id` | 36 ms   | 126 ms    |
| Tempo real do processo      | 02.35 s | 30.18 s   |


#### _Google Hash Code 2020_

Falemos agora sobre o problema do _Google Hash Code 2020_. O programa original,
escrito em `Haskell`, foi desenvolvido durante a competição, que durou quatro
horas, e está estruturado simplesmente como a composição de 3 passos -- ler o
_input_, resolver o problema, e escrever o _output_ -- como se pode verificar
no código:

```hs
main = interact (outputToString . solve . readLibraries)
```

A conversão em `C++` segue a mesma estrutura, como se pode também verificar no
código:

```cpp
output_to_string(solve(read_libraries()));
```

Tem apenas duas pequenas excepções: enquanto que em `Haskell` temos as
seguintes funções:

```hs
readLibraries :: String -> Libraries
outputToString :: Output -> String
```

Em `C++` temos dois procedimentos:

```cpp
struct libraries read_libraries (void);
void output_to_string (output_t output);
```

Isto porque seria mais difícil implementar de uma forma mais funcional e o
resultado seria muito menos idiomático -- estranho, até.

A conversão "imediata" para `C++`, com a biblioteca _"Functional Plus"_,
demorou duas tardes a completar, um total de cerca de oito horas. Para alguns
dos ficheiros de _input_, o programa em `C++` dá um resultado ligeiramente
diferente do original. Acreditamos que isto se deve a diferenças entre as
implementações do algoritmo de ordenação nas duas linguagens.

Quanto a performance, o programa original demora cerca de 7 segundos para
processar todos os ficheiros de _input_, e o programa em `C++` demora cerca de
30 minutos. Pensamos que esta elevada diferença se deve ao facto de as estruturas usadas
em `C++` não serem adequadas para o uso que lhes estamos a dar -- existe muita
cópia de memória.

## Aspectos Importantes de Programação Funcional

Neste capítulo detalharemos as características da programação funcional, mencionadas na introdução.

Composição é, provavelmente, o mais importante e talvez o único aspecto inerente a Programação Funcional. 
A ideia central de Programação Funcional é que construindo peças pequenas,
fáceis de entender e de provar como correctas, é também "simples" construir um
sistema complexo, correctamente.

De seguida, imutabilidade, em que objectos não são alterados mas sim copiados, para implementar mudanças. 
Esta propriedade ajuda a evitar erros comuns em Programação Imperativa, causados pela partilha de memória e a não
especificação da relação entre estados.

Lazy Evaluation, não sendo adoptada como estratégia de avaliação, pode ser
usada como estratégia de optimização, especialmente quando combinada com
imutabilidade e partilha de memória.

Finalmente, ADTs (_Algebraic Data Types_) são um forma de definir formalmente
novos tipos de dados a partir de tipos já existentes. Apesar de não serem
essenciais para a Programação Funcional, é desejavel criar abstrações no
sistema de tipos que ajudem a descrever o problema com que nos deparamos, dando
significado a valores e tentando limitar o conjunto de valores possíveis aos
estritamente válidos.

A seguir, para cada um destes pontos, mostraremos e analisaremos exemplos de como se faz em
`Haskell` e como se pode fazer em `C++`. 

### Imutabilidade

Uma das características mais importantes do paradigma funcional, nomeadamente na linguagem Haskell (existem linguagens funcionais impuras) é a noção de imutabilidade das expressões.
Isto faz com que não seja possível alterar o valor de variáveis já existentes mas sim, criar novas variáveis com os novos valores.

A linguagem `C++` tenta também lidar com esta noção de imutabilidade.
A noção de funções puras é dada pela avaliação de \textit{referential transparency}.
Uma função é referencialmente transparente se para o mesmo input, a função devolve sempre o mesmo valor de retorno, ou seja, substituindo uma expressão pelo seu
valor de retorno, o seu significado permanece inalterado. Por exemplo:

\begin{verbatim}
    int g = 0;

    int ref_trans(int x) {
        return x + 1;
    } // referencialmente transparente
\end{verbatim}


\begin{verbatim}
    int not_ref_trans(int x) {
        g++;
        return x + g;
    } // não referencialmente transparente(cada vez que a função é invocada, 
         tem um valor de retorno diferente)
\end{verbatim}

Se uma expressão é referencialmente transparente, não tem efeitos colaterais observáveis e, portanto, todas as funções usadas nessa expressão são consideradas puras.
A ideia de imutabilidade é particularmente útil em ambientes em que se gera concorrência, pois, existem variáveis partilhadas que podem gerar comportamentos inesperados nos programas se não for devidamente protegida a sua alteração.
Em `C++` está disponível a keyword \textit{const} que permite controlar a imutabilidade de uma variável.
Ao declarar uma variável \textit{const x} estamos a dizer ao compilador que esta variável é imutável e, qualquer tentativa de alteração à variável irá originar um erro.
De seguida analisamos a declaração de uma variável \textit{const} e os possíveis erros que podem ser cometidos ao tentar manipular essa variável.

\begin{verbatim}
    const std::string name{"John Smith"};
    
    1 - std::string name_copy = name;
    2 - std::string& name_ref = name; // erro
    3 - const std::string& name_constref = name;
    4 - std::string* name_ptr = &name; // erro
    5 - const std::string* name_constptr = &name;
\end{verbatim}

Em 1 não há ocorrências de erros pois apenas se está a associar o valor de name a uma nova variável. Em 2 teremos um erro de compilação pois estamos a passar \textit{name} por referência a uma variável não \textit{const}, situação que é resolvida em 3.
Em 4 voltamos a ter um erro de compilação pois estamos a criar um pointer não \textit{const} para name.
Este erro é resolvido em 5.
O facto de em 2 e 5 ocorrer um erro de compilação deve-se ao facto de _name_ref_ e _name_ptr_ não estarem qualificados com \textit{const} e poderem ser alterados. No entanto, como apontam para uma variável \textit{const}, gera-se uma contradição.

### Lazy Evaluation

Lazy evaluation é uma técnica de programação que adia a avaliação de uma expressão até que, e se, o seu valor for realmente necessário.
Além disso, é possível evitar a reavaliação das expressões. 
Muitas vezes, o resultado da avaliação de uma expressão é comum a outras operações. Se todas essas operações realizassem a avaliação da mesma expressão, o sistema seria sobrecarregado descenessariamente, resultando numa perda de performance.
Por exemplo, no caso de um algoritmo que recorra ao calculo do produto entre 2 matrizes A e B com alguma ou muita frequência, _lazy evaluation_ propõe calcular uma única vez o produto das matrizes e reutilizar o resultado sempre que o produto seja utilizado. Deste modo evita-se o custo computacional associado à repetição da mesma operação, o que contribui para o aumento da performance.


#### Laziness em `C++`

`C++` não é _lazy by default_ . Como tal, deverá ser o programador a aplicar esta técnica.

Vejamos um exemplo de como implementar _lazy evaluation_ em `C++`, sendo necessário ter em atenção os seguintes pontos:


  * Função sobre a qual queremos adiar o cálculo
  * Uma flag que indica se já calculamos o resultado da função.
  * O resultado calculado.

```cpp
template <typename F>
class lazy_funcall
{
    const F func;
    typedef decltype(func()) RetType;
    mutable std::optional<RetType> ret;
    mutable std::once_flag call_once_flag;

public:
    lazy_funcall (F f) : func(f) { }

    const RetType & operator() () const
    {
        std::call_once(call_once_flag, [this] { ret = func(); });
        return ret.value();
    }

};
```

### Composição

Uma parte importante de Programação Funcional é a composição de funções. Ao
escrever funções pequenas e genéricas e ao reutilizá-las com composição, é
possivel escrever programas completos rapidamente e com menos bugs. 
Em linguagens funcionais composição é usada frequentemente. Numa linguagem como `C++` não é muito
conveniente usar composição em todo o lado, principalmente por causa da sintaxe
e da semantica de passar variáveis por valor ou referência. Há um sitio, no
entanto, onde composição não tem de ser pointwise: trabalhar com colecções.
Quando há um conjunto de operações a fazer numa colecção, seja no seu todo ou parte dela, 
expressar estas operações com algum tipo de pipeline é
bastante intuitivo, legível e barato em número de caracteres escritos. Esta
ideia não é nova. Em linguagens funcionais este conceito é normalmente
implementado como listas em linguagens lazy-by-default, como Haskell, ou
lazy-lists/streams em linguagens eager-by-default, como Scheme.

Existem muitas operações sobre colecções que podem ser mapeadas numa pipeline,
sendo muitas delas bastante comuns. Programá-las de cada vez, manualmente como
um loop é tedioso e muito provavelmente menos legível do que simplesmente usar
as abstracções. Algumas destas operações comuns incluem somar, multiplicar,
filtrar, mapear e o canivete suíço, com o qual muitas das outras operações são
implementadas, o fold (tambem comummente conhecido como reduce, mas com
semantica ligeiramente diferente).

A STL de `C++` já tem algumas destas operações. Para os casos mais simples e
comuns estas podem ser suficientes. Definitivamente é melhor do que escrever um
loop à mao. Estas podem, no entanto, ser melhoradas, e a biblioteca Ranges
faz isso mesmo em dois aspectos muito importantes: usabilidade e performance.

Como exemplo, dado um vector (ou uma lista em Haskell), filtrar os elementos
dado um predicado, aplicar uma funcao a cada um deles, e depois multiplicar os
resultados pode ser feito assim em Haskell:

```hs
product . map mapper . filter pred $ xs
```

Um loop for em `C++` escrito a mao podia ser escrito como se segue:

```cpp
for (auto x : xs) {
    if (pred(x)) {
        ret *= mapper(x);
    }
}
```

Mas nao e preciso escrever loops for a mao grande parte das vezes! Era bom se
fosse possivel escrever o seguinte:

```cpp
xs | filter(pred) | transform(mapper) | product
```

E com a biblioteca Ranges e! Perceber como funciona internamente pode dar
alguma luz sobre o porque de ter melhor performance em comparacao com a STL.

Na STL, as funcoes tem como parametros dois iterators, -- o inicio e fim da
coleccao de input, ou de parte dela -- um iterator para o inicio da coleccao de
output, e um inserter, que define como os elementos serao inseridos na coleccao
de output. De imediato, alguns pontos a melhorar saltam a vista:

 * Porque e que e preciso passar os iteradores de inicio e fim da coleccao de
   input? Na verdade sabemos para que serve, mas nao precisa de ser assim. Da
   mais trabalho passar dois argumentos em vez de um, mas mais importante, e
   possivel passar iteradores para o inicio e fim de duas coleccoes diferentes
   por engano.

 * Passar iterador e inserter da coleccao de output tambem e tedioso; mas pior,
   significa que e sempre criada uma coleccao de output.

A Ranges melhora estes dois aspectos ao simplesmente abstrair coleccoes como
_ranges_, e devolver _ranges_ como resultado das operaç. Pode-se pensar
nesta abstraccao de _range_ como um par de iteradores: o inicio e fim.

Agora e possivel passar a uma operação uma coleccao, que e automaticamente
transformada num range, ou o resultado de uma outra operação. Esta ultima parte
e crucial, porque significa que podemos compor operações pointfree.

Usabilidade esta explicada. Vamos agora a porformance. O par de iteradores que
forma um range e so um par de apontadores. Para operações que nao alteram a
coleccao original nao e preciso copiar nada. Para implementar, por exemplo, o
`filter`, basta implementar o operador `++` (_next_) para o range de output,
sobre o iterador de inicio, procurando pelo elemento seguinte no range de input
que satisfaz o predicado. Se nenhum elemento satisfaz o predicado chegamos
eventualmente ao fim da coleccao, ou seja, temos um range vazio.

Quando ha a necessidade de alterar o range de input temos duas opcoes: copiar o
range de input, ou mutar a coleccao original _in-place_.

Todas as operações sao lazy, ou acontecem _on-demand_, como em Haskell -- se o
resultado nao for usado, nao se faz nada.


### ADTs

ADTs (_Algebraic Data-Types_), ou Tipos de Dados Algebricos, são
tipos de dados criados a partir de tipos já existentes, de duas maneiras
diferentes. Vamos dar uma breve descrição, para completude, simplesmente porque
nem todas as linguagens funcionais têm ADTs nativamente ou com este nome.

A primeira, e mais comum, é o produto. Dados dois tipos $A$ e $B$, o produto
deles, $A \times B$, é simplesmente o produto cartesiano entre $A$ e $B$.

A segunda, presente em grande parte das linguagens, mesmo que indirectamente, é
o co-produto. Dados dois tipos $A$ e $B$, o co-produto deles, $A + B$, é o conjunto cujos
elementos ou são do tipo $A$ ou do tipo $B$, mas é possivel distingui-los -- união disjunta. 
Este conjunto pode ser representado indirectamente como $Bool \times (A \cup
B)$: um elemento de $A$ ou $B$, e uma flag a indicar se é de $A$ ou de $B$.
Note-se que esta flag indica na verdade se o elemento é da esquerda ou direita;
$A + A$ é um tipo válido.

Com estas duas técnicas de composição é possível representar qualquer estrutura
de dados. Será então útil saber como usar estas duas técnicas numa linguagem de
programação. Em Haskell, com o seu sistema de tipos avançado, âmbas estão
disponíveis nativamente. Em `C++`, tal como em C, só o produto está disponível,
sob a forma de _structs_. Na STL também há `std::pair` e `std::tuple` que podem 
considerar-se alternativas a _structs_ em alguns casos.

De seguida vamos apresentar as três formas de compor tipos em `C++`, com as
_keywords_ `struct`, `enum`, e `union`, qual o equivalente em `Haskell` e como
cada uma se relaciona com ADTs.

##### `struct`

Por exemplo, para representar um filme, com o seu título (`String`), ano de
lançamento (`Int`), e uma pontuação (`Float`), podemos definir o tipo `Filme`
como o produto dos seus três atributos, ou seja $Filme \cong String \times Int
\times Float$.

Em `Haskell` existem várias maneiras de definir o tipo `Filme`.

```hs
type Filme = (String, Int, Float)

data Filme = Filme String Int Float

data Filme = Filme {
    titulo :: String,
    ano :: Int,
    pontuacao :: Float
}
```

A primeira reflecte mais directamente o tipo teórico; a segunda é uma definição
mais comum; a terceira, com _records_, dá "nomes" aos vários campos e é mais parecida com uma definição em `C++`.

Em `C++`, existem duas alternativas:

```cpp
struct Filme {
	std::string titulo;
	unsigned ano;
	float pontuacao;
};

typedef std::tuple<std::string, unsigned, float> Filme;
```

A primeira, que é mais idiomática, e a segunda, que é mais parecida com o tipo
teórico, como a primeira definição em `Haskell`.

##### `enum`

Um exemplo simples e conhecido a todos do uso de _enums_ é na definição do tipo
dos booleanos: `enum bool { false, true };`{.cpp} em `C++`, e `data Bool =
False | True`{.hs} em `Haskell`.

Se pensarmos nos valores de falso e verdadeiro como pertencentes a um conjunto
singular, e denotarmos esse conjunto por `false` e `true` respectivamente,
podemos pensar no tipo booleano como o co-produto de `false` e `true`, isto é, $Bool
\cong False + True$.

Poderíamos assim achar que `enum` em `C++` serve para representar co-produtos em geral mas estariamos errados. 
`enum` serve apenas para representar o co-produto de
vários conjuntos singulares ou um único conjunto enumerável de
valores não inteiros e sem ordem. Veremos mais a frente como representar tipos
de soma.

##### `union`

Esta é a menos comum das três _keywords_, por ser de uso muito limitado, e não
existe equivalente em `Haskell`. Esta "falha" do lado de `Haskell` na verdade
nao é grave -- possivelmente nem sequer é uma falha. Ao contrário do que o nome
sugere, `union` não serve para representar a união de tipos, e não vamos aqui
listar os seus usos além do necessário para este texto.

`union` pode ser usada quando se pretende guardar qualquer um de vários
valores, mas não vários em simultâneo. Por exemplo, se se pretender um tipo para
guardar ou inteiros ou _floats_, pode-se usar a seguinte `union`:

```cpp
union {
	int i;
	float f;
}
```

##### ADTs em `C++`

Vamos agora, finalmente, descrever como implementar ADTs em `C++`. A maneira
mais idiomática, possível também em `C`, é usar uma _tagged union_.

Como exemplo, vamos definir um tipo de árvores binárias de nós, com valores nos
nós e nas folhas: $BTree\ A \cong A + (A \times BTree\ A \times BTree\ A)$.

```hs
data BTree a = Leaf a
             | Node a (BTree a) (Btree a)
```

```cpp
template <typename A>
struct BTree {
	enum {
		BTree_Leaf,
		BTree_Node,
	} variant;

	union {
		A leaf;
		struct {
			A x;
			BTree<A> left;
			BTree<A> right;
		} node;
	} tree;
};
```

Esta definição em `C++` é muito maior do que a definição em `Haskell`, não só
devido à verbosidade de `C++`, como à necessidade de usar o truque
mencionado acima de transformar um co-produto num produto, ou seja,

$$BTree\ A \cong A + (A \times BTree\ A \times BTree\ A) \cong Bool \times (A\
\cup\ (A \times BTree\ A \times BTree\ A))$$

Ou, para aproximar melhor a implementação,

$$BTree\ A \cong \{\ Leaf,\ Node \ \} \times (A\ \cup\ (A \times BTree\ A
\times BTree\ A))$$

Neste caso, a `union` está realmente a simular a união de conjuntos.


Uma alternativa à tagged union, é usar `std::variant`, como a que se segue:

```cpp
template <typename A>
struct Node {
	A x;
	BTree<A> left;
	BTree<A> right;
};

template <typename A>
using BTree = std::variant<A, struct Node>;
```

### Conclusão
Ao longo deste documento, é possível constatar visualmente as diferenças sintáticas entre as duas linguagens. Em `Haskell` o código é bastante mais conciso
do que em `C++` pelo que, a sua leitura e compreensão se torna mais simpática.
Relativamente a eficiência e usabilidade das linguagens, em `Haskell` torna-se mais simples escrever programas relativamente eficientes com "pouco cuidado" uma vez que não há necessidade de preocupação com certos detalhes de implementação -- gestão de memória, ordem de execução, etc.
Embora `C++` não tenha sido inicialmente pensado para o paradigma funcional, é de notar que têm sido incluídas nas suas revisões alguns conceitos directamente relacionados com este mesmo paradigma, sendo por isso natural que em próximas revisões, a afinidade com este paradigma seja reforçada.
Relativamente ao nosso projecto, este permitiu-nos ter uma perspectiva sobre uma linguagem que até então nos era desconhecida, `C++`, além de nos ter permitido aprofundar alguns conceitos do paradigma funcional.
Notámos, no entanto, que existe muito mais conteúdo a ser explorado no âmbito deste tema, tal como _Concorrência_, _Monads_, _Error Handling_, etc cuja investigação poderá ser realizada em projectos futuros, ou mesmo por outros alunos, dando continuidade ao material já existente.




[^awkward_squad]: Ver _[Tackling the Awkward Squad]_.
[^cpp_prelude]: Ver _[CPP Prelude]_.
[^fplus]: Ver _[Functional Plus]_.
[^let_over_lambda]: Para mais informação sobre este assunto, ler [_Let Over Lambda_](https://letoverlambda.com).

[CPP Prelude]: https://github.com/kdungs/cpp-prelude
[Functional Plus]: https://github.com/Dobiasd/FunctionalPlus
[Tackling the Awkward Squad]: https://www.microsoft.com/en-us/research/publication/tackling-awkward-squad-monadic-inputoutput-concurrency-exceptions-foreign-language-calls-haskell
