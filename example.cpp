#include "gtest/gtest.h"
#include "gmock/gmock.h"


int convert(const char s);

int arabian(const std::string & s)
{
    int sum = 0;
    for(auto c:s)
        sum += convert(c);
    return sum;
}

int convert(const char s) {
    switch(s) {
        case 'I' :
            return 1;
        case 'V' :
            return 5;
        case 'X' :
            return 10;
        case 'L' :
            return 50;
        case 'C' :
            return 100;
        case 'D' :
            return 500;
        case 'M' :
            return 1000;

        default :
            return 0;
    }
}


class ArabianShould : public ::testing::TestWithParam<std::tuple<std::string, int>> {

};

TEST_P(ArabianShould, singleCharacterOfDictionary)
{
    std::tuple<std::string, int> const & params = GetParam();
    std::string const & s = std::get<0>(params);
    int expected = std::get<1>(params);
    ASSERT_EQ(expected, convert(s.at(0)));

}

TEST(ArabianShould, returnTwo)
{
    ASSERT_EQ(2, arabian("II"));
}

TEST(ArabianShould, returnThree)
{
    ASSERT_EQ(3, arabian("III"));
}

INSTANTIATE_TEST_CASE_P(returnResult, ArabianShould, testing::Values(
        std::make_tuple("I",1),
        std::make_tuple("V",5),
        std::make_tuple("X",10),
        std::make_tuple("L",50),
        std::make_tuple("C",100),
        std::make_tuple("D",500),
        std::make_tuple("M",1000)
));





static std::string fizzBuzz(int x)
{
    if (x != 3)
        return std::to_string(x);
    return "FIZZ";
}

// Simple test, does not use gmock


TEST(FizzBuzzShould, returnFIZZ)
{
    EXPECT_EQ("FIZZ", fizzBuzz(3));
}

// Real class we want to mock
class TeaBreak
{
public:
    virtual ~TeaBreak() {}

    // Return minutes taken to make the drinks
    int morningTea()
    {
        return makeCoffee(true,  1) +
               makeCoffee(false, 0.5) +
               makeHerbalTea();
    }

private:
    virtual int makeCoffee(bool milk, double sugars) = 0;
    virtual int makeHerbalTea() = 0;
};

// Mock class
class MockTeaBreak : public TeaBreak
{
public:
    MOCK_METHOD2(makeCoffee,    int(bool milk, double sugars));
    MOCK_METHOD0(makeHerbalTea, int());
};


using ::testing::Return;
using ::testing::_;

// Mocked test
TEST(TeaBreakTest, MorningTea)
{
MockTeaBreak  teaBreak;
EXPECT_CALL(teaBreak, makeCoffee(_,_))
.WillOnce(Return(2))
.WillOnce(Return(1));
EXPECT_CALL(teaBreak, makeHerbalTea())
.WillOnce(Return(3));

EXPECT_LE(teaBreak.morningTea(), 6);
}


class FizzBuzzShould : public ::testing::TestWithParam<int> {


};

TEST_P(FizzBuzzShould, returnResult)
{
    int x = GetParam();
    ASSERT_EQ(std::to_string(x), fizzBuzz(x));

}

INSTANTIATE_TEST_CASE_P(returnResult, FizzBuzzShould, testing::Values(0, 1, 2, 4));
