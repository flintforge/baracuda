/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.
 */
#ifndef TIME_H
#define TIME_H

class Time
{
public:
    static int time;
    static int factor;
    static void setTime(int t) { Time::time = t*factor; }
    static void setFactor(int f) { Time::factor = f; }
};

#endif // TIME_H
