package base;

import redis.clients.jedis.HostAndPort;
import redis.clients.jedis.Jedis;

public class Main {

    public static void main(String[] args) {
        final var node = new HostAndPort("192.168.0.6", 6379);

        Jedis jedisCluster = new Jedis(node);

        jedisCluster.set("key1", "value1");
        System.out.println(jedisCluster.get("key1"));
    }
}