

class Node {
public:

    const int a = 2;

    virtual int evaluate() = 0;
private:

};

class Number : public Node {
public:

    constexpr Number(const int _value) : value (_value) {}

    const int value;

    int evaluate() {
        return 2;
    }

private:

};

/*
To make value1 and value2 constant we gotta provide 
a assign operator to play nice with the parser
*/

class BinOp {
public:

    constexpr BinOp(const int _value1, const int _value2) : value1 (_value1), value2 (_value2) {}

    int value1;
    int value2;

    constexpr int evaluate() const {
        return value1 + value2;
    }


private:

};
