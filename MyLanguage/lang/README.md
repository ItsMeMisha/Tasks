# lang

ЯВУ. который помимо написания программ позволяет переводить текст программы в мелодию.

Пример программы на этом языке:
```
legato SubTheme [dominanta] ||

    nota abc = $E1 = sqrt 5|

    ||:[abc meno 5] abc = abc + 1| :||

    DS sqrt 4|

||

legato MainTheme [] ||

    nota abc = $A0 = 3|

    volta [abc meno 5] ||
      abc = abc - 1|

    || secvolta ||
      abc = 0|

    ||

    SubTheme [abc]|

    DS|

||
```

Здесь `legato` - объявление функции, `nota` - объявление переменной. Значения `$E1` и `$A0` определяют, какой ноте соответствует объявленная переменная.

Такая конструкция соответствует `if-else`:

```
volta [condition] ||
...
|| secvolta ||
...
||
```

А такая соответствует циклу `while`:

```
||:[condition]
...
:||
```

`DS` соотетсвует `return`.
