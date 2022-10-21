package org.example;

import redis.clients.jedis.Jedis;

import java.util.logging.Logger;


public class Taxi extends RedisClient {

    public Taxi(final Jedis jedis) {
        super(jedis);
    }

    /*
    Приклад виконання Redis команди:
    execute("SET", "key", "123");
    Ви можете використовувати змінні в командах:
    public void set(String key, String value) {
        execute("SET", key, value);
    }
    Також ви можете повертати значення:
    public String get(String key) {
        return execute("GET", key);
    }
    */

    public void userRegistered(String username, String name, String age) {
        //Цей метод повинен зберігати користувача
        execute("HSET", username, "name", name, "age", age);
    }

    public String getUser(String username) {
        //Цей метод повинен повертати дані про користувача
        return execute("HGETALL", username);
    }

    public void carRegistered(String longitude, String latitude, String carName) {
        //Цей метод повинен зберігати машину та її розташування
        try
        {
            execute("GEOADD", "Cars:Map", longitude, latitude, carName);
            execute("HSET", "Cars:Free", carName, "1");
        }
        catch(Exception e)
        {
            System.out.println(e.getMessage());
        }

    }

    public void carTaken(String carName) {
        //Цей метод має помічати машину як зайняту
        execute("HDEL", "Cars:Free", carName);
    }

    public String getFreeCars() {
        //Цей метод має повертати не зайняті машини
        int free = 0;
        String output = null;
        try
        {
            output = execute("HGETALL",
                "Cars:Free"     // all cars key
                );
        }
        catch(Exception e)
        {
            System.out.println(e.getMessage());
        }

        return output;
    }

    public String locateCar(String carName) {
        //Цей метод повинен повертати розташування машини
        String output = null;

        try
        {
            output = execute("GEOSEARCH",
                    "Cars:Map",
                    "FROMMEMBER", carName,
                    "BYRADIUS", "200", "km",
                    "WITHCOORD");
        }
        catch(Exception e)
        {
            System.out.println(e.getMessage());
        }

        return output;
    }

    public void orderReceived(String order) {
        //Цей метод повинен додавати замовлення в чергу
        try
        {
            execute("RPUSH", "constumer:oders", order);
        }
        catch(Exception e)
        {
            System.out.println(e.getMessage());
        }
    }

    public String selectOder() {
        //Цей метод має повертати перше замовлення з черги
        String output = null;

        try
        {
            output = execute("LPOP", "constumer:oders");
        }
        catch(Exception e)
        {
            System.out.println(e.getMessage());
        }

        return output;
    }

    public void scoreCalculated(String username, String userScore) {
        //Цей метод має зберігати рейтинг користувача
        try
        {

            execute("ZADD", "constumer:rate", userScore, username);
        }
        catch(Exception e)
        {
            System.out.println(e.getMessage());
        }
    }

    public String getBestCustomer() {
        //Цей метод має повертати користувача з найбільшим рейтингом
        String output = null;
        try
        {
            output = execute("ZREVRANGE", "constumer:rate", "0", "0");
        }
        catch(Exception e)
        {
            System.out.println(e.getMessage());
        }

        return output;
    }
}