#include "OrderBook.h"

#include <algorithm>

OrderBook::OrderBook() : _orders()
{
}

OrderBook::OrderBook(const OrderBook& other) : _orders(other._orders)
{
}

OrderBook::~OrderBook()
{
}

OrderBook& OrderBook::operator=(const OrderBook& other)
{
    if (this != &other)
    {
        _orders = other._orders;
    }
    return *this;
}

void OrderBook::ProcessOrder(const Order& order)
{
    if (order._type == OrderType::Add)
    {
        AddOrder(order);
    }
    else
    {
        RemoveOrder(order._id, order._timestamp);
    }
}

void OrderBook::AddOrder(const Order& order)
{
    if (_orders.empty())
    {
        _currentStartInterval = order._timestamp;
        _currentPrice = order._price;
    }
    else if (_currentPrice != order._price)
    {
        auto interval = order._timestamp - _currentStartInterval;
        _currentDivider += interval;
        _maxPrice += _currentPrice * interval;
        _currentStartInterval = order._timestamp;
        _currentPrice = order._price;
    }
    _orders.emplace_back(order);
}

void OrderBook::RemoveOrder(std::uint32_t id, std::uint32_t timestamp)
{
    auto it = std::find_if(_orders.cbegin(), _orders.cend(), [&](const Order& order) {
        return id == order._id;
    });
    if (_currentPrice != (*it)._price)
    {
        auto interval = _prevTimestamp - _currentStartInterval;
        _currentDivider += interval;
        _maxPrice += _currentPrice * interval;
        _currentStartInterval = _prevTimestamp;
        _currentPrice = (*it)._price;
    }
    if (it != _orders.cend())
    {
        _prevTimestamp = timestamp;
        _orders.erase(it);
    }
    if (_orders.empty())
    {
        auto interval = _prevTimestamp - _currentStartInterval;
        _currentDivider += interval;
        _maxPrice += _currentPrice * interval;
        _currentStartInterval = 0;
        _currentPrice = 0.0;
    }
}

double OrderBook::MaxPrice() const
{
    return _maxPrice / _currentDivider;
}