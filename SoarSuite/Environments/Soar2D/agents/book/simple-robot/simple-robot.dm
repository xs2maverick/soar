110
SOAR_ID 0
SOAR_ID 1
SOAR_ID 2
SOAR_ID 3
ENUMERATION 4 1 state
ENUMERATION 5 1 nil
SOAR_ID 6
ENUMERATION 7 2 complete error
SOAR_ID 8
ENUMERATION 9 3 backward forward stop
ENUMERATION 10 2 complete error
ENUMERATION 11 3 left right stop
SOAR_ID 12
STRING 13
SOAR_ID 14
ENUMERATION 15 1 wander
SOAR_ID 16
ENUMERATION 17 1 state
ENUMERATION 18 1 wander
ENUMERATION 19 1 move
SOAR_ID 20
ENUMERATION 21 2 operator state
ENUMERATION 22 2 multiple none
ENUMERATION 23 2 no-change tie
ENUMERATION 24 1 book
SOAR_ID 25
ENUMERATION 26 1 wait
ENUMERATION 27 1 elaboration
SOAR_ID 28
SOAR_ID 29
ENUMERATION 30 1 forward
ENUMERATION 31 1 rotate
SOAR_ID 32
ENUMERATION 33 1 stop
SOAR_ID 34
SOAR_ID 35
SOAR_ID 36
FLOAT_RANGE 37 -Infinity Infinity
INTEGER_RANGE 38 0 2147483647
SOAR_ID 39
SOAR_ID 40
INTEGER_RANGE 41 0 2147483647
SOAR_ID 42
INTEGER_RANGE 43 0 2147483647
INTEGER_RANGE 44 0 2147483647
SOAR_ID 45
INTEGER_RANGE 46 0 2147483647
INTEGER_RANGE 47 0 2147483647
SOAR_ID 48
FLOAT_RANGE 49 0.0 Infinity
FLOAT_RANGE 50 0.0 Infinity
FLOAT_RANGE 51 -Infinity Infinity
ENUMERATION 52 4 east north south west
SOAR_ID 53
SOAR_ID 54
FLOAT_RANGE 55 -Infinity Infinity
FLOAT_RANGE 56 0.0 Infinity
FLOAT_RANGE 57 0.0 Infinity
ENUMERATION 58 4 east north south west
INTEGER_RANGE 59 0 2147483647
SOAR_ID 60
INTEGER_RANGE 61 0 2147483647
INTEGER_RANGE 62 0 2147483647
SOAR_ID 63
INTEGER_RANGE 64 0 2147483647
INTEGER_RANGE 65 0 2147483647
INTEGER_RANGE 66 0 2147483647
SOAR_ID 67
ENUMERATION 68 2 false true
ENUMERATION 69 2 false true
INTEGER_RANGE 70 0 2147483647
INTEGER_RANGE 71 -2147483648 2147483647
SOAR_ID 72
FLOAT_RANGE 73 0.0 Infinity
FLOAT_RANGE 74 0.0 Infinity
INTEGER_RANGE 75 0 2147483647
INTEGER_RANGE 76 0 2147483647
FLOAT_RANGE 77 0.0 1.0
FLOAT_RANGE 78 0.0 Infinity
SOAR_ID 79
FLOAT_RANGE 80 0.0 Infinity
FLOAT_RANGE 81 -Infinity Infinity
FLOAT_RANGE 82 -Infinity Infinity
SOAR_ID 83
SOAR_ID 84
FLOAT_RANGE 85 -Infinity Infinity
FLOAT_RANGE 86 -Infinity Infinity
ENUMERATION 87 2 complete error
ENUMERATION 88 2 complete error
ENUMERATION 89 2 complete error
ENUMERATION 90 1 move-to-gateway
SOAR_ID 91
SOAR_ID 92
ENUMERATION 93 1 state
ENUMERATION 94 1 move-to-gateway
ENUMERATION 95 1 rotate
SOAR_ID 96
ENUMERATION 97 1 move
SOAR_ID 98
ENUMERATION 99 1 get
SOAR_ID 100
ENUMERATION 101 1 drop
SOAR_ID 102
ENUMERATION 103 1 move-block
SOAR_ID 104
SOAR_ID 105
ENUMERATION 106 1 state
ENUMERATION 107 1 move-block
ENUMERATION 108 1 blank
SOAR_ID 109
114
0 attribute 21
0 choices 22
0 impasse 23
0 io 1
0 name 24
0 operator 14
0 operator 25
0 superstate 5
0 top-state 0
0 type 4
1 input-link 2
1 output-link 3
2 self 35
3 move 6
3 rotate 8
3 rotate-absolute 83
3 rotate-relative 84
3 stop-sim 12
6 direction 9
6 status 7
8 direction 11
8 status 10
12 status 89
12 true 13
14 actions 3
14 name 15
16 name 18
16 operator 20
16 operator 32
16 operator 34
16 operator 91
16 operator 100
16 operator 102
16 operator 104
16 superstate 0
16 top-state 0
16 type 17
20 actions 28
20 name 19
25 name 26
25 random 27
28 move 29
29 direction 30
32 name 31
34 name 33
35 angle 36
35 area-description 39
35 collision 67
35 cycle 70
35 position 72
35 random 77
35 score 71
35 time 78
35 velocity 79
36 yaw 37
39 door 53
39 wall 40
40 area 38
40 center 48
40 direction 52
40 id 41
40 left 42
40 right 45
42 col 43
42 row 44
45 col 47
45 row 46
48 angle-off 51
48 x 49
48 y 50
53 center 54
53 direction 58
53 id 59
53 left 60
53 right 63
53 to 66
54 angle-off 55
54 x 56
54 y 57
60 col 61
60 row 62
63 col 64
63 row 65
67 x 69
67 y 68
72 col 76
72 row 75
72 x 73
72 y 74
79 speed 80
79 x 81
79 y 82
83 status 87
83 yaw 85
84 amount 86
84 status 88
91 name 90
92 name 94
92 operator 96
92 operator 98
92 superstate 16
92 top-state 0
92 type 93
96 name 95
98 name 97
100 name 99
102 name 101
104 name 103
105 name 107
105 operator 109
105 superstate 16
105 top-state 0
105 type 106
109 name 108
