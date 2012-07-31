
union intstructure {
    int i;
    struct breakout {
        bool b1 : 1;
        bool b2 : 1;
        bool b3 : 1;
        bool b4 : 1;
        bool b5 : 1;
        bool b6 : 1;
        bool b7 : 1;
        bool b8 : 1;
        bool b9 : 1;
        bool b10 : 1;
        bool b11 : 1;
        bool b12 : 1;
        bool b13 : 1;
        bool b14 : 1;
        bool b15 : 1;
        bool b16 : 1;
    } bf;
};

void printintstructure(const intstructure& is);

void setup() {
    Serial.begin(115200);
    
    Serial.print("    1/0 = ");
    Serial.println(1/0);
    Serial.print("    0/0 = ");
    Serial.println(0/0);

    intstructure s1, s2, s3, s4, s5, s6;
    s1.i = 3;
    Serial.print("      3 = ");
    printintstructure(s1);

    s2.i = 4820;
    Serial.print("   4820 = ");
    printintstructure(s2);
    
    s3.i = 1/0;
    Serial.print("    1/0 = ");
    printintstructure(s3);

    s4.i = 2948/0;
    Serial.print(" 2948/0 = ");
    printintstructure(s4);

    s5.i = 0/0;
    Serial.print("    0/0 = ");
    printintstructure(s5);
    
    Serial.print("-3932/0 = ");
    s6.i = -3932/0;
    printintstructure(s6);
}

void printintstructure(const intstructure& is) {
    Serial.print(is.bf.b16);
    Serial.print(is.bf.b15);
    Serial.print(is.bf.b14);
    Serial.print(is.bf.b13);
    Serial.print(is.bf.b12);
    Serial.print(is.bf.b11);
    Serial.print(is.bf.b10);
    Serial.print(is.bf.b9);
    Serial.print(is.bf.b8);
    Serial.print(is.bf.b7);
    Serial.print(is.bf.b6);
    Serial.print(is.bf.b5);
    Serial.print(is.bf.b4);
    Serial.print(is.bf.b3);
    Serial.print(is.bf.b2);
    Serial.print(is.bf.b1);
    Serial.println();
}

void loop() {

}
