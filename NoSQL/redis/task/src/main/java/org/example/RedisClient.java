package org.example;

import java.util.Arrays;
import java.util.stream.Collectors;

import redis.clients.jedis.Jedis;

public class RedisClient {

    private final Jedis jedis;

    public RedisClient(Jedis jedis) {
        this.jedis = jedis;
    }

    protected String execute(String command, String... parameters) {

        String actual = Arrays.stream(parameters)
                              .map(word -> "'" + word + "'")
                              .collect(Collectors.joining(", "));

        Object response = jedis.eval("return redis.call('" + command + "',"  + actual + ")");

        if (response != null) {
            return response.toString();
        }

        return null;
    }
}
