 # スパゲッティコード的な電卓のコード

括弧がある四則演算の数式が計算できる。（途中計算付き）

````
./a.out "3*(1+2+3+4)/2.5"

1.000000 + 2.000000
3.000000 + 3.000000
6.000000 + 4.000000
3.000000 * 10.000000
30.000000 / 2.500000

Ans = 12.000000
````

ゼロ割の時にはエラーとする。
```
./a.out "1/0"
1.000000 / 0.000000

Error Div by Zero
```
