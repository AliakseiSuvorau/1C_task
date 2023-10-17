Суворов Алексей Сергеевич

Задача 2 (алгоритмы)
## Вход
В первой строке идет число M - количество ребер

Далее идет M строк, в каждой из которых написано 2 числа - вершины, соединенные этим ребром.
Количество вершин не превышает $`10^6`$. 

(Этот предел можео увеличить, изменив константу kMaxVertex внутри класса, но это приведет к замедлению программы. Поэтому было бы лучше просто вводить с клавиатуры количество вершин)

## Выход
В первой строке - найденное число вершин. В следующей строке - все эти вершины.

## Идея
1) Заразим все вершины степени менее 2, так как их по-другому заразить не удастся. В полученном графе у всех незараженных вершин есть хотя бы 2 соседа.
2) Раскрасим граф правильно, используя жадный алгоритм. Пусть получилось n цветов. Теперь выберем (n-1) цвет так, чтобы множество вершин, окрашенных в эти цвета, было минимально по мощности. Заразим все эти вершины. 
Пусть после этого у нас в графе остались (даже через соседей) незараженные вершины. Выберем одну. Если она будет иметь 2 соседей. Пусть один из них не заражен. Но тогда он того же цвета - противоречие. То есть заражены (прямо или косвенно) все вершины.

Очевидно, что найденное после этого множество вершин будет неминимально. Попробуем улучшить результат.
3) Пройдемся во всем зараженным вершинам (именно непременно зараженны, а не зараженным соседями) и если у какой-то зараженной так вершины будет $`\geq2`$ зараженных соседа, то вылечим ее. Получим лучшее при данной раскраске множество вершин.

## Асимптотика
Пусть n - количество вершин, m - количество ребер
1) Ввод: $`O(m)`$
2) Определение вершин степени меньше 2.
 * Прошлись по всем вершинам и сделали проверку. Т.е. $`O(n)`$.
3) Раскраска графа. Пусть $`m_i`$ - число ребер, исходящих из $`i`$-й вершины.
 * Прошлись по вешинам и пометили, какие цвета заняты - за $`O(m_i)`$ итераций. Каждая итерация - $`O(\log n)`$, так как вставляем в set.
 * Для каждой вершины нашли еще не занятый цвет - за $`O(m_i)`$ итераций.
 * Присваивание нового цвета вершине и увеличение счетчика - $`O(\log n)`$
 * В итоге все за $`O(m + m\log n + n\log n)=O((m + n)\log n)`$
4) Выбор цвета с максимальным числом вершин - $`O(n)`$.
5) Вылечивание ненужных. Бежим по вершинам, проверяем всех соседей и лечим ненужных - $`O(m + n\log n)`$
6) В итоге сложность всего алгоритма получается: $`O((n + m)\log n)`$

## Запуск
```
g++ main.cpp -o main
./main
```

## Замечания
* Можно улучшить точность (при этом проиграв по времени), если найти все вершины, которые станут зараженнвми из-за своих соседей за один обход (потом - за 2, 3 и т.д. до n). В конце концов мы исключим все "лишние" вершины из множества инфицированных. Асимптотика такого усложнения будет $`O(nm)`$, так как мы на каждой из n итераций переберем все ребра в графе. Итераций же всего n - так как мы любой путь содержит не более n различных вершин.
* Таким образом, алгоритм делает неплохую оценку сверху на количество вершин
