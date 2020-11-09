#pragma once

#include <boost/asio.hpp>
#include <boost/thread.hpp>

class AsyncExec {
public:
    AsyncExec(){}
    AsyncExec(const boost::function<void()> &fn) : t_(fn) {}
    void exec(const boost::function<void()> &fn) { t_ = boost::move(boost::thread(fn)); }
    void wait() { if (t_.joinable()) { t_.join(); } }
    void detach() { t_.detach(); }

private:
    boost::thread t_;
};

class SimpleTimer {
public:
    typedef boost::function<void()> OnTimerFunc;
    SimpleTimer(int millisecond) : timeout_(millisecond), timer_(io_, timeout_)
    {
        timer_.async_wait(boost::bind(&SimpleTimer::onTimer, this, _1));
        thread_ = boost::move(boost::thread(boost::bind(&boost::asio::io_service::run, &io_)));
    }
    ~SimpleTimer()
    {
        if (!io_.stopped()) {
            io_.stop();
        }

        if (thread_.joinable()) {
            thread_.join();
        }
    }

    void setOnTimer(const OnTimerFunc &func)
    {
        onTimer_ = func;
    }

private:
    void onTimer(const boost::system::error_code &e)
    {
        if (onTimer_) {
            onTimer_();
        }

        timer_.expires_at(timer_.expires_at() + timeout_);
        timer_.async_wait(boost::bind(&SimpleTimer::onTimer, this, _1));
    }

private:
    boost::posix_time::millisec timeout_;
    boost::asio::io_service io_;
    boost::asio::deadline_timer timer_;
    boost::thread thread_;
    OnTimerFunc onTimer_;
};

