% Paradigma Funcional em Haskell e C++
% André Sá (A76361) ; João Rodrigues (A84505) ; Pedro Oliveira (A86328)
% 2020/07/22 \linebreak\linebreak Licenciatura em Ciências da Computação

# Introdução

O paradigma funcional tem ganho notorieadade junto de grandes empresas e
programadores em detrimento de outros, pois permite que em poucas linhas de
código, quando comparado com outros estilos, se consiga criar soluções robustas
e eficientes.

Neste documento será dada ênfase às vantagens do paradigma funcional e de que
forma podemos aproveitar essas vantagens em `C++`. Iremos estudar e analisar as
características funcionais em programas escritos em `C++`, através de algumas
bibliotecas existentes para esse efeito e, aproveitaremos para efectuar uma
análise comparativa de performance, sintaxe, etc, através de programas que
resolvem o mesmo problema em `C++` e `Haskell`.

O uso de `template`s em `C++` traz algumas vantagens à programação em estilo
funcional, nomeadamente a possibilidade de fazer programação genérica, isto é,
criar programas polimórficos. Também é possível obter computação em tempo de
compilação com `template`s, mas esta não é essencial a Programação Funcional, e
portanto não vamos desenvolver sobre este assunto.[^let_over_lambda]

Aproveitaremos também para aprofundar alguns aspectos/características
importantes da Programação Funcional tais como:

 * Imutabilidade
 * Lazy Evaluation
 * Composição
 * ADTs

Quando necessário, e para uma melhor elucidação sobre as questões que estão a
ser analisadas, serão usados pequenos excertos de código em ambas as
linguagens.

# Abordagem ao Paradigma Funcional em `Haskell` e `C++`

## O paradigma funcional

Na programação funcional, os programas são executados através da avaliação de
expressões, em contraste, por exemplo, com o paradigma imperativo onde os
programas são compostos por instruções que vão alterando o estado global à
medida que executam. Isto significa que os procedimentos podem ter acesso ao
estado global e/ou partilhado entre varios procedimentos. Esta partilha não
está especificada de forma nenhuma e, portanto, tem de ser o programador a
cuidar e evitar que problemas aconteçam. O paradigma Funcional evita este
problema parcial ou completamente, ao desencorajar ou impedir esta prática e,
ao mesmo tempo, encorajar e facilitar "boa pratica".

\pagebreak

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

Por outro lado, numa linguagem funcional escreveríamos (em notacao `Haskell`)
`accoes = accao3 . accao2 . accao1`{.hs} para representar a mesma sequência de
acções mas sem partilha de memória nem estruturas de dados a serem mutadas:
cada uma das acções é uma função que devolve uma estrutura de dados, dada outra
estrutura de dados.

Este problema de alteração implicita de estado agrava-se ainda mais num
contexto concorrente com threads e partilha de memoria.

## `Haskell` como linguagem funcionalmente pura

`Haskell` adopta o paradigma Funcionalmente Puro, o que quer dizer que um
programa é uma função no sentido matemático, ou seja, dado o mesmo _input_ é
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

## `C++ "funcional"`

Devido à sua herança, `C++` promove um estilo frágil de programação, devendo
ser o programador a ter alguma atenção e a tomar algumas decisões quando
pretende usar o paradigma funcional em `C++`. Por exemplo:

 * Evitar dados mutáveis. Numa função que altera uma estrutura, em vez de
   receber a estrutura por referência e a alterar, será melhor receber a
   estrutura por valor e devolver uma nova. Por razões de performance, também
   pode ser boa ideia passar a estrutura por referencia `const`{.c}, que se
   traduz em menos movimentação de memória.

 * Para um estilo de programação mais genérico, mas ao mesmo tempo mais seguro,
   preferir `template`{.cpp}s a `void *`{.cpp}, o que permite uma abstração de
   tipos, indo de encontro ao que acontece em `Haskell`. Vejamos o exemplo de
   uma função que soma dois valores passados como argumento.

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
   `auto`{.cpp}.

 * Recorrer ao uso de _lambdas_ para criar abstrações (desde `C++11`).

 * Utilizar bibliotecas funcionais existentes, como _"Functional
   Plus"_[^fplus], _"CPP Prelude"_[^cpp_prelude], ou _"Ranges"_[^ranges].

# Comparação e Análise de Programas Equivalentes em `Haskell` e `C++`

Neste capítulo, faremos uma comparação mais específica sobre programas escritos
em âmbas as linguagens e cujo propósito é o mesmo, ou seja, podem considerar-se
equivalentes. Durante a pesquisa que efectuamos, encontramos duas bibliotecas
que tentam transpôr o paradigma funcional para `C++`, que vão de encontro aos
objectivos do nosso projeto. Vamos começar por algumas funções sobre listas do
_prelude_ do `Haskell`, usando a biblioteca _"CPP Prelude"_, para uma
comparação mais directa, e terminaremos com um programa mais robusto que foi
utilizado na ronda de qualificação do _Google Hash Code 2020_, do qual tinhamos
a versão em `Haskell` e fizemos a conversão para `C++` utilizando a biblioteca
_"Functional Plus"_, para uma comparação mais realista.

## _Prelude_

De forma a comparar a performance de pequenos programas em âmbas as linguagens,
geramos um ficheiro de _input_ com uma lista de 10000000 de inteiros. Note-se
que deixamos de fora da análise o processo de leitura do ficheiro. Focaremos a
comparação na aplicação de funções específicas em `Haskell` e `C++`. Para cada
função, vamos apresentar uma definição com recursividade explícita e uma
definição recorrendo a funções de ordem superior em `Haskell`, seguidas de uma
implementação em `C++` e no final apresentamos os tempos de execução.

### `map`

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
map f = foldr (\a -> (f a :)) []
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
em `C++`. Como devolve uma colecção, temos de criar uma a colecção de resultado
(`res`{.cpp}) -- em `Haskell` isto é feito de forma automática.

### `filter`

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
filter p = foldr (\a -> if p a then (a:) else id) []
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
`std::copy_if`{.cpp}. Apesar de não sabermos à partida quantos elementos terá a
colecção de resultado, por razões de performance, podemos na mesma reservar
espaço. No fim, a colecção pode conter menos elementos que os reservados, e
para remover a memória inutilizada, usa-se `shrink_to_fit`{.cpp}.

\pagebreak

### `reverse`

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

### `zip`

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

Neste caso, não existe nenhuma função parecida na STL, e portanto, é definida
manualmente como um ciclo.

### Resultados

Para comparar performance entre as duas linguagens, medimos o tempo de CPU de
cada função, com os meios disponíveis em cada uma. Simultaneamente medimos o
tempo de execução real do processo com o programa `/usr/bin/time`{.sh}.

Em `C++` usamos `std::clock()`{.cpp} do _header_ `<ctime>`{.cpp}, com o seguite
macro:

```cpp
#define benchmark(str, func)                   \
  do {                                         \
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

\pagebreak

Em `Haskell` usamos `getCPUTime`{.hs} de `System.CPUTime`{.hs}, com a seguinte
função:

```hs
timeSomething :: NFData a => String -> a -> IO ()
timeSomething str something = do
  start <- liftIO getCPUTime
  let !result = deepforce $! something
  end <- liftIO getCPUTime
  let diff = round . (/1000) . fromIntegral $ end - start
  putStrLn $ str ++ ": " ++ show diff ++ " nanoseconds"
```

Como `Haskell` é _lazy-by-default_, para obter-mos uma comparação justa é
necessário forçar a avaliação das expressões que pretendemos testar. Para isso
usamos o `deepforce`{.hs}, que está definido como `deepforce x = deepseq x
x`{.hs}, sendo `deepseq a b`{.hs} a função que força a avaliação de `a`{.hs} e
devolve `b`{.hs}.

|                             | `C++`   | `Haskell` |
| :-------------------------: | :-----: | :-------: |
| `map (*2)`                  | 14 ms   | 149 ms    |
| `filter even`               | 48 ms   | 139 ms    |
| `reverse`                   | 11 ms   | 806 ms    |
| `uncurry zip . split id id` | 36 ms   | 126 ms    |
| Tempo real do processo      | 02.35 s | 30.18 s   |

## _Google Hash Code 2020_

Falemos agora sobre o problema do _Google Hash Code 2020_. O problema é de
optimização, e consiste em planear que livros serão examinados e de que
biblioteca, de forma a maximizar a pontuação. Não iremos detalhar aqui o
problema em si, os ficheiros de input e output, -- visto que estão disponíveis
na página da competição[^hash_code_2020] -- nem a estratégia usada para o
resolver, visto não estar directamente relacionado com o tema deste trabalho.

O programa original, escrito em `Haskell`, foi desenvolvido durante a
competição, que durou quatro horas, e está estruturado simplesmente como a
composição de três passos -- ler o _input_, resolver o problema, e escrever o
_output_ -- como se pode verificar no código:

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

Em `C++` temos estes dois procedimentos:

```cpp
struct libraries read_libraries (void);
void output_to_string (output_t output);
```

Isto porque seria mais difícil implementar de uma forma mais funcional e o
resultado seria muito menos idiomático -- estranho, até.

### Notas Sobre Funções Relevantes Utilizadas

#### `fplus::fwd::apply`

Esta função é usada para aplicar funções sobre colecções a uma colecção. Por
exemplo, quando em `Haskell` escreveríamos `func3 . func2 . func1 $ col`{.hs},
em `C++`, com a biblioteca _"Functional Plus"_, escrevemos
`fplus::fwd::apply(col, func1, func2, func3)`{.cpp}.

Funções uteis para usar com `fplus::fwd::apply` podem ser encontradas no
_namespace_ `fplus::fwd`.

#### `fplus::transform`

Esta função serve um propósito similar ao da função `map`{.hs} em `Haskell`.
Enquanto que em `Haskell` escreveríamos `map func col`{.hs}, em `C++`, com a
biblioteca _"Functional Plus"_, escrevemos `fplus::transform(func, col)`{.cpp}.

#### `fplus::keep_if`

Esta função serve um propósito similar ao da função `filter`{.hs} em `Haskell`.
Enquanto que em `Haskell` escreveríamos `filter pred col`{.hs}, em `C++`, com a
biblioteca _"Functional Plus"_, escrevemos `fplus::keep_if(pred, col)`{.cpp}.

#### `fplus::fwd`

Como em `C++` não há _auto-currying_, de forma a ser possível compor funções da
biblioteca, existem no _namespace_ `fplus::fwd` versões para este propósito.
Por exemplo, podemos escrever

```cpp
fplus::fwd::apply(col,
                 fplus::fwd::keep_if(pred),
                 fplus::fwd::transform(func));
```

em vez de

```cpp
fplus::transform(func, fplus::keep_if(pred, col));
```

#### `std::tie`


Para destruturar tuplos em `C++` -- mas não pares -- podemos usar
`std::tie`{.cpp}. Isto é equivalente a fazer _pattern match_ sobre um tuplo em
`Haskell`. Vejamos um exemplo:

Em `Haskell`:

```hs
t :: (Int, Float, Char, String, Double)
t = (0, 1, 'a', "ola", 2)
(x, _, _, _, d) = t
```

Em `C++`:

```cpp
std::tuple<int, float, char, std::string, double> t
    = std::make_tuple(0, 1, 'a', "ola", 2);
int x;
double d;
std::tie(x, std::ignore, std::ignore, std::ignore, d) = t;
```

### Tipos e Estruturas de Dados

Definimos apenas três tipos para uso no programa, -- um para representar o
_input_, outro para representar o _output_, e um para representar uma
biblioteca -- e as únicas estruturas de dados usadas foram listas e vectores --
só vectores do lado de `C++`. Apresentamos as suas definições a seguir.

Em `Haskell`:

```hs
--                  ID    nBooks  Signup books/day books
type LibraryDesc = (Int, (Int,    Int,   Int,      V.Vector Int))

data Libraries = Libraries {
  nBooks :: Int,
  nLibraries :: Int,
  nDays :: Int,
  bookScore :: V.Vector Int,
  libraries :: [LibraryDesc]
  } deriving Show

--                        lib  nb   books
newtype Output = Output [(Int, Int, V.Vector Int)]
```

Em `C++`:

```cpp
typedef std::pair<int, std::tuple<int, int, int, std::vector<int>>>
        library_desc_t;

struct libraries {
  int n_books;
  int n_libraries;
  int n_days;
  std::vector<int> book_scores;
  std::vector<library_desc_t> libraries;
};

typedef std::vector<std::tuple<int, int, std::vector<int>>> output_t;
```

### Ler o _Input_

Para a leitura do _input_ tiramos proveito da _lazyness_ do `Haskell`, e
tomamos o _input_ como uma `String`{.hs}. Do lado do `C++` usamos também
`std::string`{.cpp}, simplesmente porque pretendiamos uma conversão mais
directa -- esta não é a melhor escolha para performance, mas para os ficheiros
de _input_ não é expectável qualquer penalização, visto que o maior destes tem
apenas 3.4MB.

De seguida apresentamos o ficheiro de exemplo mais pequeno:

```
6 2 7       -- 6 livros, 2 bibliotecas, 7 dias
1 2 3 6 5 4 -- pontuação de cada livro
5 2 2       -- A biblioteca 0 tem 5 livros, 2 dias de signup, 2 livros/dia
0 1 2 3 4   -- Os livros que a biblioteca 0 tem
4 3 1       -- A biblioteca 1 tem 4 livros, 3 dias de signup, 1 livro/dia
0 2 3 5     -- Os livros que a biblioteca 1 tem
```

Em `Haskell`:

```hs
onTSBD (_, (_, ts, bd, _)) = (ts, bd)

readLibraries :: String -> Libraries
readLibraries = proc . map (map read . words) . lines
  where
    proc ([nb, nl, nd]:scores:library_desc) = Libraries nb nl nd scs libs
      where
        scs = V.fromList scores
        libs = (proc3 . zip [0..] $ proc2 library_desc)

    proc3 = sortOn onTSBD

    proc2 ([nb, su, bd]:books:t) = (nb, su, bd, V.fromList books):(proc2 t)
    proc2 _ = []
```

Em `C++`:

```cpp
struct libraries read_libraries (void)
{
  std::string input(std::istreambuf_iterator<char>{std::cin}, {});
  std::vector<std::vector<int>> values = fplus::fwd::apply(
      input,
      fplus::fwd::split_lines(false),
      fplus::fwd::transform([](std::string line) -> std::vector<int> {
        return fplus::fwd::apply(
            line,
            fplus::fwd::split_words(false),
            fplus::fwd::transform(fplus::read_value_unsafe<int>));
        }));

  struct libraries ret;

  { /* Primeira linha */
    ret.n_books = values[0][0];
    ret.n_libraries = values[0][1];
    ret.n_days = values[0][2];
  }

  { /* Segunda linha */
    ret.book_scores = values[1];
  }

  { /* Resto */
    ret.libraries.reserve(ret.n_libraries);
    for (int i = 0; i < ret.n_libraries; i++) {
      std::vector<int> props = values[2 * i + 2 + 0];
      std::vector<int> books = values[2 * i + 2 + 1];

      int n_books = props[0];
      int sign_up = props[1];
      int books_per_day = props[2];

      ret.libraries.push_back(
          std::make_pair(
            i,
            std::make_tuple(n_books,
              sign_up,
              books_per_day,
              books)));
    }
  }

  ret.libraries = fplus::sort_on(&on_ts_bd, ret.libraries);

  return ret;
}
```

Como se pode verificar, o passo de separar a _string_ de _input_ em linhas e
palavras, e de ler essas palavras para inteiros, está muito parecido ao
original -- a definição da variável `values`{.cpp}. O passo `zip [0..] .
proc2`{.hs} já foi fundido num só _loop_ _for_.

### Resolver o Problema

Em `Haskell`:

```hs
ordBookScore :: Libraries -> Int -> Int
ordBookScore l id = (V.! id) $ bookScore l

solve :: Libraries -> Output
solve l = cenas . sortOn onTSBD $ solve' (nDays l) (distinct $ libraries l)
  where
    distinct :: [LibraryDesc] -> [LibraryDesc]
    distinct = map mapper . filter pred . fst . foldl' folder def
      where
        mapper (id, (nb, ts, bd, bs)) = (id, (nb, ts, bd, sorted))
          where
            sorted = V.fromList . sortOn (ordBookScore l) $ V.toList bs

        pred (_, (_, _, _, bs)) = not $ V.null bs

        def = ([], S.empty)
        folder (ret, s) (id, (nb, ts, bd, bs)) = let
          bss = S.fromList $ V.toList bs
          bs' = S.difference bss s
          ss = S.union s bs'
          bs'' = V.fromList $ S.toList bs'
          in ((id, (S.size bs', ts, bd, bs'')):ret, ss)

    cenas = Output . map (\(id, (nb, ts, bd, bs)) -> (id, nb, bs))

    solve' 0 _ = []
    solve' _ [] = []
    solve' nd (e@(_, (_, ts, _, _)):t)
      | ts <= nd = e:(solve' (nd - ts) t)
      | otherwise = solve' nd t
```

Em `C++`:

```cpp
/* Esta função representa a função `distinct` acima */
struct libraries distinct (struct libraries libs)
{
  libs.libraries = fplus::fwd::apply(
      libs.libraries,
      fplus::fwd::fold_left(
        [](std::pair<std::vector<library_desc_t>, std::set<int>> a,
          library_desc_t b) {
        std::vector<library_desc_t> ret = a.first;
        std::set<int> s = a.second;

        int id = b.first;
        int ts, bd;
        std::vector<int> bs;
        std::tie(std::ignore, ts, bd, bs) = b.second;

        std::set<int> bss(bs.begin(), bs.end());
        std::set<int> bs_ = fplus::set_difference(bss, s);
        std::set<int> ss = fplus::set_merge(s, bs_);

        return std::make_pair(
            fplus::prepend_elem(
              std::make_pair(
                id,
                std::make_tuple(
                  bs_.size(),
                  ts,
                  bd,
                  std::vector<int>(bs_.begin(), bs_.end()))),
              ret),
            ss);
        },
          std::make_pair(std::vector<library_desc_t>(), std::set<int>())),
          fplus::fwd::fst(),
          fplus::fwd::keep_if([](library_desc_t pt)
              { return !std::get<3>(pt.second).empty(); }),
          fplus::fwd::transform(
              fplus::fwd::transform_snd(
                [libs](std::tuple<int, int, int, std::vector<int>> tup) {
                int nb, ts, bd;
                std::vector<int> bs;
                std::tie(nb, ts, bd, bs) = tup;
                bs = fplus::sort_on([libs](int bid) { return libs.book_scores[bid]; }, bs);
                return std::make_tuple(nb, ts, bd, bs);
                }))
  );
  return libs;
}

/* Esta função representa a função `solve'` acima */
std::vector<library_desc_t>
solve_ (int n_days, std::vector<library_desc_t> libs)
{
  std::vector<library_desc_t> ret;
  int len = libs.size();
  assert(len > 0);

  for (int i = 0; i < len && n_days > 0; i++) {
    library_desc_t e = libs[i];
    /*
     * `e` é um `std::pair`
     * `e.second` acede à componente da direita.
     *
     * `e.second` é um `std::tuple`
     * `std::get<1>` acede à componente de índice 1
     */
    int ts = std::get<1>(e.second);

    if (ts <= n_days) {
      ret.push_back(e);
      n_days -= ts;
    }
  }

  return ret;
}

output_t solve (struct libraries libs)
{
  libs = distinct(libs);
  return fplus::fwd::apply(
      solve_(libs.n_days, libs.libraries),
      fplus::fwd::sort_on(&on_ts_bd),
      fplus::fwd::transform(
        [](library_desc_t pt) {
          int id = pt.first;
          int nb;
          std::vector<int> bs;
          std::tie(nb, std::ignore, std::ignore, bs) = pt.second;
          return std::make_tuple(id, nb, bs);
        }));
}
```

### Escrever o _Output_

Aqui apresentamos o _output_ gerado pelos dois programas, dado o ficheiro de
exemplo mostrado acima:

```
2           -- 2 bibliotecas
0 5         -- 5 livros, da biblioteca 0
0 1 2 4 3   -- Livros 0, 1, 2, 4, 3, por esta ordem
1 1         -- 1 livro, da biblioteca 1
5           -- Livro 5
```

Em `Haskell`:

```hs
outputToString :: Output -> String
outputToString (Output libs) = unlines
                             . ((show nLibs):)
                             $ concatMap mapper libs
  where
    mapper = map (unwords . map show)
           . (\(x, y, l) -> [[x, y], V.toList l])
    nLibs = length libs
```

Em `C++`:

```cpp
void output_to_string (output_t output)
{
  /* Esta operação representa `((show nLibs):)` acima */
  std::cout << output.size() << std::endl;

  /* O corpo deste ciclo representa a função `mapper` acima */
  for (const std::tuple<int, int, std::vector<int>> & lib : output) {
    int x, y;
    std::vector<int> l;
    std::tie(x, y, l) = lib;

    std::cout << x << " " << y << std::endl;

    int len = l.size();
    assert(len > 0);
    std::cout << l[0];

    for (int i = 1; i < len; i++)
      std::cout << " " << l[i];

    std::cout << std::endl;
  }
}
```

### Resultados

A conversão "imediata" para `C++`, com a biblioteca _"Functional Plus"_,
demorou duas tardes a completar, um total de cerca de oito horas. Relativamente
a performance, apresentamos de seguida a tabela com os tempos de processamento
por cada ficheiro.

| Ficheiro                       | `Haskell` | `C++`      | _Output_ igual |
| :----------------------------: | :-------: | :--------: | :------------: |
| `a_example.txt`                | 0s        | 0s         | Sim            |
| `b_read_on.txt`                | 0.7s      | 9.95s      | Não            |
| `c_incunabula.txt`             | 1.26s     | 4m 40.83s  | Sim            |
| `d_tough_choices.txt`          | 1.57s     | 23m 59.30s | Não            |
| `e_so_many_books.txt`          | 3.01s     | 3m 5.39s   | Não            |
| `f_libraries_of_the_world.txt` | 2.9s      | 3m 13.55s  | Sim            |
| Total                          | 9.44s     | 35m 9.02s  |

Como é possível verificar na tabela, o tempo total de execução para todos os
ficheiros de _input_ é muito superior em `C++`. Pensamos que esta diferença
acentuada se deve ao facto de as estruturas usadas em `C++` não serem adequadas
para o uso que lhes estamos a dar -- existe muita cópia de memória. Por outro
lado, a biblioteca que estamos a utilizar poderá ter sido pensada para satisfazer
os requisitos funcionais descurando performance.

Para alguns dos ficheiros de _input_, o programa em `C++` dá um resultado
ligeiramente diferente do original. Como os programas são deterministas,
acreditamos que isto se deve a diferenças entre as implementações do algoritmo
de ordenação nas duas linguagens.

# Aspectos Importantes de Programação Funcional

Neste capítulo detalharemos as características da programação funcional,
mencionadas na introdução.

Composição é, provavelmente, o mais importante e talvez o único aspecto
inerente a Programação Funcional. A ideia central de Programação Funcional é
que construindo peças pequenas, fáceis de entender e de provar como correctas,
é também "simples" construir um sistema complexo, correctamente.

De seguida, imutabilidade, em que objectos não são alterados mas sim copiados,
para implementar mudanças. Esta propriedade ajuda a evitar erros comuns em
Programação Imperativa, causados pela partilha de memória e a não especificação
da relação entre estados.

Lazy Evaluation, não sendo adoptada como estratégia de avaliação, pode ser
usada como estratégia de optimização, especialmente quando combinada com
imutabilidade e partilha de memória.

Finalmente, ADTs (_Algebraic Data Types_) são um forma de definir formalmente
novos tipos de dados a partir de tipos já existentes. Apesar de não serem
essenciais para a Programação Funcional, é desejavel criar abstrações no
sistema de tipos que ajudem a descrever o problema com que nos deparamos, dando
significado a valores e tentando limitar o conjunto de valores possíveis aos
estritamente válidos.

A seguir, para cada um destes pontos, mostraremos e analisaremos exemplos de
como se faz em `Haskell` e como se pode fazer em `C++`.

## Imutabilidade

Uma das características mais importantes do paradigma funcional, é a noção de
imutabilidade das expressões. Isto faz com que não seja possível alterar o
valor de variáveis já existentes mas sim, criar novas variáveis com os novos
valores.

A linguagem `C++` tenta também lidar com esta noção de imutabilidade. A noção
de funções puras é dada pela avaliação de _referential transparency_. Uma
função é referencialmente transparente se para o mesmo _input_, é sempre
devolvido o mesmo valor de retorno -- ou seja, substituindo uma expressão pelo
seu valor de retorno, o seu significado permanece inalterado -- e não tem
efeitos colaterais observáveis. Por exemplo:

```cpp
int g = 0;

/* Referencialmente transparente */
int ref_trans (int x) {
    return x + 1;
}

/*
 * Não referencialmente transparente -- cada vez que a função é
 * invocada, tem um valor de retorno diferente
 */
int not_ref_trans1 (int x) {
    g++;
    return x + g;
}

/*
 * Não referencialmente transparente -- embora o valor de retorno seja sempre o
 * mesmo, dado o mesmo input, há um efeito colateral, o incremento de `g`
 */
int not_ref_trans2 (int x) {
    g++;
    return x + 1;
}
```

A ideia de imutabilidade é particularmente útil em ambientes em que se gera
concorrência, pois, existem variáveis partilhadas que podem gerar
comportamentos inesperados nos programas se não for devidamente protegida a sua
alteração. Em `C++` está disponível a keyword `const`{.cpp} que permite
controlar a imutabilidade de uma variável. Ao declarar uma variável `const
x`{.cpp} estamos a dizer ao compilador que esta variável é imutável e, qualquer
tentativa de alteração à variável irá originar um erro de compilação.  De
seguida analisamos a declaração de uma variável `const`{.cpp} e os possíveis
erros que podem ser cometidos ao tentar manipular essa variável.

\pagebreak

```cpp
const std::string name{"John Smith"};

1 - std::string name_copy = name;
2 - std::string& name_ref = name; // erro
3 - const std::string& name_constref = name;
4 - std::string* name_ptr = &name; // erro
5 - const std::string* name_constptr = &name;
```

Em 1 não há ocorrências de erros pois apenas se está a associar o valor de name
a uma nova variável. Em 2 teremos um erro de compilação pois estamos a passar
`name`{.cpp} por referência a uma variável não `const`{.cpp}, situação que é
resolvida em 3. Em 4 voltamos a ter um erro de compilação pois estamos a criar
um apontador não `const`{.cpp} para `name`{.cpp}. Este erro é resolvido em 5. O
facto de em 2 e 5 ocorrer um erro de compilação deve-se ao facto de
`name_ref`{.cpp} e `name_ptr`{.cpp} não estarem qualificados com `const`{.cpp}
e poderem ser alterados. No entanto, como apontam para uma variável
`const`{.cpp}, gera-se uma contradição.

## _Lazy Evaluation_

_Lazy Evaluation_ é uma técnica de programação que adia a avaliação de uma
expressão até que, e se, o seu valor for realmente necessário. Além disso, é
possível evitar a reavaliação de uma expressão.

Muitas vezes, o resultado da avaliação de uma expressão é comum a várias
operações. Se todas essas operações avaliassem a expressão, o sistema seria
sobrecarregado descenessariamente, resultando numa perda de performance. Por
exemplo, no caso de um algoritmo que recorra ao cálculo do produto entre duas
matrizes com alguma frequência, _lazy evaluation_ propõe calcular uma única vez
o produto das matrizes e reutilizar o resultado sempre que o produto seja
utilizado. Deste modo evita-se o custo computacional associado à repetição da
mesma operação, o que contribui para o aumento da performance.

`C++` não é _lazy-by-default_, e como tal, deverá ser o programador a aplicar
esta técnica.

Vejamos uma possível implementação de _lazy evaluation_ em `C++`, sendo
necessário ter em atenção os seguintes pontos:

 * Função sobre a qual queremos adiar o cálculo
 * Uma _flag_ que indica se já calculamos o resultado da função
 * O resultado calculado

\pagebreak

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

## Composição

Uma parte importante de Programação Funcional é a composição de funções. Ao
escrever funções pequenas e genéricas, e ao reutilizá-las com composição, é
possível escrever programas completos rapidamente e com menos bugs. Em
linguagens funcionais, composição é usada frequentemente; numa linguagem como
`C++` não é muito conveniente usar composição em todo o lado, principalmente
por causa da sintaxe e da semântica de passar variáveis por valor ou
referência. Há um sitio, no entanto, onde composição não tem de ser
_pointwise_: trabalhar com colecções. Quando há um conjunto de operações a
aplicar a uma colecção, seja no seu todo ou parte dela, expressar estas
operações com algum tipo de _pipeline_ é bastante intuitivo, legível e barato
em número de caracteres escritos. Esta ideia não é nova -- em linguagens
funcionais este conceito é normalmente implementado como listas em linguagens
_lazy-by-default_, como `Haskell`, ou _lazy-lists_/_streams_ em linguagens
_eager-by-default_, como `Scheme`.

Existem muitas operações sobre colecções que podem ser mapeadas numa
_pipeline_, sendo muitas delas bastante comuns. Programá-las de cada vez
manualmente como um _loop_ é tedioso e muito provavelmente menos legível do que
simplesmente usar as abstracções. Algumas destas operações comuns incluem
somar, multiplicar, filtrar, mapear e o canivete suíço, com o qual muitas das
outras operações são implementadas, o `fold` -- também comummente conhecido
como `reduce`, mas com semântica ligeiramente diferente.

A STL de `C++` já tem algumas destas operações. Para os casos mais simples e
comuns estas podem ser suficientes. É definitivamente melhor do que escrever um
_loop_ manualmente. Estas podem, no entanto, ser melhoradas. Existem várias
bibliotecas que implementam conceitos funcionais em `C++`; vamos usar apenas a
_"Functional Plus"_ no documento. No entanto, existe uma outra biblioteca
parecida, _"Ranges"_, com melhor performance, mas a documentação é escassa, o
que torna difícil perceber como a usar.

Como exemplo, dado um vector (ou uma lista em `Haskell`), filtrar os elementos
dado um predicado, aplicar uma função a cada um deles, e depois multiplicar os
resultados pode ser feito assim em `Haskell`:

```hs
product . map mapper . filter pred $ xs
```

Um _loop_ `for` em `C++` escrito manualmente podia ser como o que se segue --
omitindo a declaração e inicialização da variável `ret`{.hs}:

```cpp
for (auto x : xs) {
    if (pred(x)) {
        ret *= mapper(x);
    }
}
```

Mas não é preciso escrever _loops_ `for` manualmente grande parte das vezes --
podemos em vez disso escrever o seguinte[^fplus_examples]:

```cpp
fplus::fwd::apply(
    xs
    , fplus::fwd::keep_if(pred)
    , fplus::fwd::transform(mapper)
    , fplus::fwd::product()
    )
```

O nosso estudo não se centrou apenas na _"Functional Plus"_, no entanto. A
partir do livro _Functional Programming in C++_, do _Ivan Čukić_, pudemos obter
uma amostra do que é possível neste tipo de biblioteca. Em particular, o livro
explica por alto porque é que a biblioteca _"Ranges"_ tem melhor performance
que a STL, e que a _"Functional Plus"_.

Começando por usabilidade: na STL, as funções têm como parâmetros dois
_iterators_, -- o início e fim da colecção de _input_, ou de parte dela -- um
iterador para o início da colecção de _output_, e um _inserter_, que dita como
os elementos serão inseridos na colecção de _output_. De imediato, alguns
pontos a melhorar saltam à vista:

 1. Porque é que é preciso passar os iteradores de início e fim da colecção de
    _input_? Na verdade sabemos para que serve, mas não precisa de ser assim.
    Dá mais trabalho passar dois argumentos em vez de um, mas mais importante:
    é possível passar iteradores para o início e fim de duas colecções
    diferentes por engano.

 2. Passar iterador e _inserter_ da colecção de _output_ também é tedioso; mas
    pior, significa que é sempre criada uma colecção de _output_.

A _"Functional Plus"_ melhora o primeiro destes pontos, -- basta passar a
colecção em si, não iteradores para ela -- mas o segundo ponto continua um
problema presente -- e sempre devolvida uma nova operação como resultado.

A _"Ranges"_ melhora estes dois aspectos ao simplesmente abstrair colecções
como _ranges_, e devolver _ranges_ como resultado das operações. Pode-se pensar
nesta abstracção de _range_ como um par de iteradores: o início e fim.

Agora é possível passar a uma operação uma colecção, que é automaticamente
transformada num _range_, ou o resultado de uma outra operação, que já é um
_range_. Esta última parte é crucial -- significa que podemos compor operações
_pointfree_.

Usabilidade está explicada. Vamos agora a porformance. O par de iteradores que
forma um _range_ é só um par de apontadores. Para operações que não alteram a
colecção original não há necessidade de copiar memória. Para implementar, por
exemplo, o `filter`{.hs}, basta implementar o operador `++`{.cpp} (_next_) para
o _range_ de _output_, sobre o iterador de início, procurando pelo elemento
seguinte no _range_ de _input_ que satisfaz o predicado. Se nenhum elemento
satisfaz o predicado chegamos eventualmente ao fim da colecção, ou seja, temos
um _range_ vazio.

Quando há a necessidade de alterar o _range_ de _input_ temos duas opções:
copiar o _range_ de _input_, ou mutar a colecção original _in-place_.

Todas as operações são _lazy_, ou seja, acontecem _on-demand_, como em
`Haskell` -- se o resultado não for usado, nada é feito.

## ADTs

ADTs (_Algebraic Data-Types_), ou Tipos de Dados Algebricos, são tipos de dados
criados a partir de tipos já existentes, de duas maneiras diferentes. Vamos dar
uma breve descrição, para completude, simplesmente porque nem todas as
linguagens funcionais têm ADTs nativamente ou com este nome.

A primeira, e mais comum, é o produto. Dados dois tipos $A$ e $B$, o produto
deles, $A \times B$, é simplesmente o produto cartesiano entre $A$ e $B$.

A segunda, presente em grande parte das linguagens, mesmo que indirectamente, é
o co-produto. Dados dois tipos $A$ e $B$, o co-produto deles, $A + B$, é o
conjunto cujos elementos ou são do tipo $A$ ou do tipo $B$, mas é possível
distingui-los -- união disjunta. Este conjunto pode ser representado
indirectamente como $Bool \times (A \cup B)$: um elemento de $A$ ou $B$, e uma
flag a indicar se é de $A$ ou de $B$. Note-se que esta flag indica na verdade
se o elemento é da esquerda ou direita: $A + A$ é um tipo válido.

Com estas duas técnicas de composição é possível representar qualquer estrutura
de dados. Será então útil saber como usar estas duas técnicas numa linguagem de
programação. Em `Haskell`, com o seu sistema de tipos avançado, âmbas estão
disponíveis nativamente. Em `C++`, tal como em `C`, só o produto está
disponível, sob a forma de `struct`{.cpp}s. Na STL também existem
`std::pair`{.cpp} e `std::tuple`{.cpp} que podem considerar-se alternativas em
alguns casos.

De seguida vamos apresentar as três formas de compor tipos em `C++`, com as
_keywords_ `struct`{.cpp}/`class`{.cpp}, `enum`{.cpp}, e `union`{.cpp}, qual o
equivalente em `Haskell` e como cada uma se relaciona com ADTs.

### `struct`/`class`

Juntamos estas duas _keywords_ visto que servem o mesmo propósito; a única
diferença está em que, caso nada seja dito em contrário, numa `struct`{.cpp}
todos os membros são públicos, enquanto que numa `class`{.cpp} são privados.

Por exemplo, para representar um filme, com o seu título (`String`{.hs}), ano
de lançamento (`Int`{.hs}), e uma pontuação (`Float`{.hs}), podemos definir o
tipo `Filme`{.hs} como o produto dos seus três atributos, ou seja $Filme \cong
String \times Int \times Float$.

Em `Haskell` existem várias maneiras de definir o tipo `Filme`{.hs}.

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
mais comum; a terceira, com _records_, dá "nomes" aos vários campos e é mais
parecida com uma definição em `C++`.

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

### `enum`

Um exemplo simples e conhecido a todos do uso de _enums_ é na definição do tipo
dos booleanos: `enum bool { false, true };`{.cpp} em `C++`, e `data Bool =
False | True`{.hs} em `Haskell`.

Se pensarmos nos valores de falso e verdadeiro como pertencentes a um conjunto
singular, e denotarmos esse conjunto por `false` e `true` respectivamente,
podemos pensar no tipo booleano como o co-produto de `false` e `true`, isto é,
$Bool \cong False + True$.

Poderíamos assim achar que `enum`{.cpp} em `C++` serve para representar
co-produtos em geral mas estariamos errados. `enum`{.cpp} serve apenas para
representar o co-produto de vários conjuntos singulares ou um único conjunto
enumerável de valores não inteiros e sem ordem. Veremos mais a frente como
representar tipos de soma.

### `union`

Esta é a menos comum das três _keywords_, por ser de uso muito limitado, e não
existe equivalente em `Haskell`. Esta "falha" do lado de `Haskell` na verdade
não é grave -- possivelmente nem sequer é uma falha. Ao contrário do que o nome
sugere, `union`{.cpp} não serve para representar a união de tipos, e não vamos
aqui listar os seus usos além do necessário para este texto.

`union`{.cpp} pode ser usada quando se pretende guardar qualquer um de vários
valores, mas não vários em simultâneo. Por exemplo, se se pretender um tipo
para guardar ou inteiros ou _floats_, pode-se usar a seguinte `union`{.cpp}:

```cpp
union {
    int i;
    float f;
}
```

### ADTs em `C++`

Vamos agora, finalmente, descrever como implementar ADTs em `C++`. A maneira
mais idiomática, possível também em `C`, é usar uma _tagged union_.

Como exemplo, vamos definir um tipo de árvores binárias de nós, com valores nos
nós e nas folhas: $BTree\ A \cong A + (A \times BTree\ A \times BTree\ A)$.

Em `Haskell`:

```hs
data BTree a = Leaf a
             | Node a (BTree a) (Btree a)
```

\pagebreak

Em `C++`:

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
devido à verbosidade de `C++`, como à necessidade de usar o truque mencionado
acima de transformar um co-produto num produto, ou seja,

$$BTree\ A \cong A + (A \times BTree\ A \times BTree\ A) \cong Bool \times (A\ \cup\ (A \times BTree\ A \times BTree\ A))$$

Ou, para aproximar melhor a implementação,

$$BTree\ A \cong \{\ Leaf,\ Node \ \} \times (A\ \cup\ (A \times BTree\ A \times BTree\ A))$$

Neste caso, a `union`{.cpp} está realmente a simular a união de conjuntos.

Uma alternativa à _tagged union_, é usar `std::variant`{.cpp}, como a que se
segue:

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

# Conclusão

Ao longo deste documento, é possível constatar visualmente as diferenças
sintáticas entre as duas linguagens. Em `Haskell` o código é bastante mais
conciso do que em `C++`, pelo que a sua leitura e compreensão se torna mais
simpática. Relativamente a eficiência e usabilidade das linguagens, em
`Haskell` torna-se mais simples escrever programas relativamente eficientes com
"pouco cuidado" uma vez que não há necessidade de preocupação com certos
detalhes de implementação -- gestão de memória, ordem de execução, etc. Embora
`C++` não tenha sido inicialmente pensado para o paradigma funcional, é de
notar que têm sido incluídas nas suas revisões alguns conceitos directamente
relacionados com este mesmo paradigma, sendo por isso natural que em próximas
revisões, a afinidade com este paradigma seja reforçada.

Relativamente ao nosso projecto, este deu-nos uma amostra sobre uma linguagem
que até então nos era desconhecida, `C++`, e proporcionou-nos uma diferente
perspectiva sobre alguns conceitos do paradigma funcional. Notámos, no entanto,
que existe muito mais conteúdo a ser explorado no âmbito deste tema, tal como
_Concorrência_, _Monads_, _Error Handling_, cuja investigação poderá ser
realizada em projectos futuros, ou mesmo por outros alunos, dando continuidade
ao material já existente.

[^awkward_squad]: Ver _[Tackling the Awkward Squad]_.
[^cpp_prelude]: Ver _[CPP Prelude]_.
[^fplus]: Ver _[Functional Plus]_.
[^fplus_examples]: Para mais exemplos de uso, ver o programa do [_Google Hash Code 2020_](#_google-hash-code-2020_).
[^hash_code_2020]: O enunciado do problema e ficheiros de input podem ser descarregados daqui: [https://codingcompetitions.withgoogle.com/hashcode/archive](https://codingcompetitions.withgoogle.com/hashcode/archive)
[^let_over_lambda]: Para mais informação sobre este assunto, ler [_Let Over Lambda_](https://letoverlambda.com).
[^ranges]: Ver _[Ranges]_.

[CPP Prelude]: https://github.com/kdungs/cpp-prelude
[Functional Plus]: https://github.com/Dobiasd/FunctionalPlus
[Ranges]: https://github.com/ericniebler/range-v3
[Tackling the Awkward Squad]: https://www.microsoft.com/en-us/research/publication/tackling-awkward-squad-monadic-inputoutput-concurrency-exceptions-foreign-language-calls-haskell
