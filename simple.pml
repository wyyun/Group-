int x = 0;
int y = 1;
proctype X1()
{
    x = x + 1;
    x = x + 2
}

proctype X2()
{
    x = x + 3
}

proctype Y1()
{
    y = y + 1;
    y = y * 2
}

proctype Y2()
{
    y = y + 3
}

init {
    run X1(x);
    run X2(x);
    run Y1(y);
    run Y2(y)
}
