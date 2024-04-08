### popsicle lang
popsicle is a toy compiled language written in c, here is a simple example usage
of the language:

```
divide (int x, y) :: !int
    if x == 0 || y == 0
        return !math.ZeroDivision
    end if

    return x / y
;;

main () :: void
    let x, y = 69, 420
    let result = divide x, y

    print "result is :result:"
;;

```

