#include "main.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using ::testing::Return;

namespace
{
    class DummyTimeStamp : public TimeStamp
    {
    public:
        long long int get() const override {
            return 1;
        }
    };
    class BlogAuctionTaskTestImpl : public BlogAuctionTask
    {
        using super = BlogAuctionTask;
    public:
        BlogAuctionTaskTestImpl(const double & price, double & proposal)
                : super(std::make_unique<AveragePricer>(),
                        std::make_unique<MarketPublisher>(),
                        std::make_unique<DefaultPriceComputer>(std::make_unique<DummyTimeStamp>()))
                , m_averagePrice(price)
                , m_resultProposal(proposal)
        {}

        double retrieveAveragePrice(const std::string &blog) override {
            return m_averagePrice;
        }

        void publishProposal(const std::string &blog, double const proposal) override {
            m_resultProposal = proposal;
        }

        double & m_resultProposal;
        const double & m_averagePrice;
    };
}

class BlogAuctionShouldOdd : public testing::TestWithParam<double>
{
};

TEST_P(BlogAuctionShouldOdd, OddPriceUnderstanding)
{
    double expectedPrice = GetParam();

    double expectedProposal = 99999;

    const double seed = (expectedPrice + 2)/2. ;
    double result = seed * 6.28;
    BlogAuctionTaskTestImpl auctionTask(expectedPrice, expectedProposal);
    auctionTask.PriceAndPublish("HackerNews", "FAST");

    ASSERT_DOUBLE_EQ(result, expectedProposal);
}

INSTANTIATE_TEST_CASE_P(oddCase, BlogAuctionShouldOdd,
                        testing::Values(2., 4., 6., 8., 10.));

class BlogAuctionShouldEven : public testing::TestWithParam<double>
{
};

TEST_P(BlogAuctionShouldEven, EvenPriceUnderstanding)
{
    const double expectedPrice = GetParam();
    double expectedProposal = 99999;

    double result = 3.15;
    BlogAuctionTaskTestImpl auctionTask(expectedPrice, expectedProposal);
    auctionTask.PriceAndPublish("HackerNews", "");

    ASSERT_DOUBLE_EQ(result, expectedProposal);
}


INSTANTIATE_TEST_CASE_P(evenCase, BlogAuctionShouldEven,
                        testing::Values(1.34, 1., 3., 9., 51., 1001.));

class BlogAuctionShouldMode : public testing::TestWithParam<std::string>
{
public:
    BlogAuctionShouldMode() : modeValuesMap{{{"SLOW", 1}}} {};

private:
    std::map<std::string, int> modeValuesMap;

protected:
    int findElement(const std::string & mode) const
    {
        auto it = modeValuesMap.find(mode);

        if (it == modeValuesMap.end())
        {
            return 1;
        }
        return it->second;
    }
};

/*TEST_P(BlogAuctionShouldMode, ModePriceUnderstanding)
{
    std::string expectedMode = GetParam();

    double expectedProposal = 99999;

    BlogAuctionTaskTestImpl auctionTask(1, expectedProposal, 1);
    auctionTask.PriceAndPublish("HackerNews", expectedMode);

    auto valueMode = findElement(expectedMode);

    ASSERT_DOUBLE_EQ(result, );
}*/

std::vector<std::string> GetDataDummy()
{
    return std::vector<std::string>{{"toto", "titi", "tata", "pipo"}};
}

namespace {
    struct MockDb : public IDbBlogs
    {
        MOCK_METHOD0(listAllBlogs, std::vector<std::string>());
    };
}

TEST(ADDSpaceShould, Return)
{
    MockDb mockDb;
    std::vector<std::string> expectedReturn = GetDataDummy();
    EXPECT_CALL(mockDb, listAllBlogs()).Times(1).WillOnce(Return(expectedReturn));

    AdSpace a(mockDb);
    std::vector<std::string> adSpacesBlogs = a.listAllBlogs();

    ASSERT_EQ(GetDataDummy(), adSpacesBlogs);
}
