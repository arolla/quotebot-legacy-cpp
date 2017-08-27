#include <iostream>
#include <random>
#include <thread>
#include <unordered_map>

// External Deps
class QuotePublisher
{
public:
    static QuotePublisher& getInstance()
    {
        static QuotePublisher instance;
        return instance;
    }
private:
    QuotePublisher() {}
public:
    QuotePublisher(QuotePublisher const&)  = delete;
    void operator=(QuotePublisher const&)  = delete;
    void publish(double s) {
        std::cout << "You've pushed a dummy auction to a real ads platform, the business is upset!" << std::endl;
    }
};


class MarketStudyVendor {
public:
    double averagePrice(std::string blog) {
        if (std::getenv("license") == NULL) {
            std::cout << "Missing license !" << std::endl;
            exit(EIO);
        }

        std::uniform_real_distribution<double> unif(0, 10000);
        std::default_random_engine re;

        return std::hash<std::string>()(blog) * unif(re);
    }
};


class BlogAuctionTask {
private:
    MarketStudyVendor marketDataRetriever;

public:
    void PriceAndPublish(std::string blog, std::string mode) {
        double avgPrice = marketDataRetriever.averagePrice(blog);
        // FIXME should actually be +2 not +1
        double proposal = avgPrice + 1;
        double timeFactor = 1;
        if (mode=="SLOW") {
            timeFactor = 2;
        }
        if (mode=="MEDIUM") {
            timeFactor = 4;
        }
        if (mode=="FAST") {
            timeFactor = 8;
        }
        if (mode=="ULTRAFAST") {
            timeFactor = 13;
        }

        proposal = std::fmod(proposal, 2.0) == 0 ? 3.14 * proposal
                                                 : 3.15 * timeFactor * std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        QuotePublisher::getInstance().publish(proposal);
    }
};


class TechBlogs {
public:
    static std::vector<std::string> listAllBlogs() {
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));// Access to DB are very slow
        return { "HackerNews", "Reddit", "TechCrunch", "BuzzFeed", "TMZ", "TheHuffPost", "GigaOM"};
    }
};


class AdSpace {
public:
    static std::vector<std::string> getAdSpaces() {
        static std::unordered_map<std::string, std::vector<std::string>> cache;
        if (!cache.empty()) {
            return cache.at("blogs list");
        }
        // FIXME : only return blogs that start with a 'T'
        std::vector<std::string> listAllBlogs = TechBlogs::listAllBlogs();
        cache["blogs list"] = listAllBlogs;
        return listAllBlogs;
    }
};

class AutomaticQuoteBot {
public:
    void sendAllQuotes(std::string mode) {
        std::vector<std::string> blogs = AdSpace::getAdSpaces();
        for (std::string blog : blogs) {
            BlogAuctionTask auctionTask;
            auctionTask.PriceAndPublish(blog, mode);
        }
    }
};

int main() {
    AutomaticQuoteBot bot;
    bot.sendAllQuotes("FAST");
    return 0;
}
