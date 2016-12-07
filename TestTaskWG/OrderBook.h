#pragma once

#include <cstdint>
#include <vector>
#include <istream>

enum class OrderType : std::uint8_t
{
    Add = 0,
    Delete
};

struct Order
{
    std::uint32_t _timestamp;
    OrderType _type;
    std::uint32_t _id;
    double _price;

    friend std::istream& operator >> (std::istream& iss, Order& order)
    {
        auto typeChar = std::uint8_t{ 0 };
        auto spaceChar = std::uint8_t{ 0 };
        iss >> order._timestamp >> typeChar >> order._id >> order._price;
        switch (typeChar)
        {
        case 'I':
            order._type = OrderType::Add; break;
        case 'E':
            order._type = OrderType::Delete; break;
        default:
            break;
        }
        return iss;
    }
};

class OrderBook
{
public:
    OrderBook();
    OrderBook(const OrderBook& other);
    ~OrderBook();
    OrderBook& operator=(const OrderBook& other);

public:
    double MaxPrice() const;
    void ProcessOrder(const Order& order);

private:
    void AddOrder(const Order& order);
    void RemoveOrder(std::uint32_t id, std::uint32_t timestamp);

private:
    std::vector<Order> _orders;
    double _maxPrice;
    double _currentPrice;
    std::uint32_t _currentDivider;
    std::uint32_t _currentStartInterval;
    std::uint32_t _prevTimestamp;
};