package org.example;

import redis.clients.jedis.Jedis;

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
        //return execute("GET", username);
    }

    public void carRegistered(String longitude, String latitude, String carName) {
        //Цей метод повинен зберігати машину та її розташування
        execute("GEOADD", longitude, latitude, carName);
        execute("HSET", "car", carName, "Busy", "0");
    }

    public void carTaken(String carName) {
        //Цей метод має помічати машину як зайняту
        execute("HSET", "car", carName, "Busy", "1");
    }

    public String getFreeCars() {
        //Цей метод має повертати не зайняті машини
        int free = 0;
        execute("HSCAN", "car", "0", "Busy");
        return null;
    }

    public String locateCar(String carName) {
        //Цей метод повинен повертати розташування машини
        return null;
    }

    public void orderReceived(String order) {
        //Цей метод повинен додавати замовлення в чергу
    }

    public String selectOder() {
        //Цей метод має повертати перше замовлення з черги
        return null;
    }

    public void scoreCalculated(String username, String userScore) {
        //Цей метод має зберігати рейтинг користувача
    }

    public String getBestCustomer() {
        //Цей метод має повертати користувача з найбільшим рейтингом
        return null;
    }
}