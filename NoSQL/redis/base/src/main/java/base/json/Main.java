package base.json;

import redis.clients.jedis.HostAndPort;
import redis.clients.jedis.UnifiedJedis;
import redis.clients.jedis.json.Path;

public class Main {

    public static void main(String[] args) {
        final var node = new HostAndPort("192.168.0.6", 6379);
        UnifiedJedis client = new UnifiedJedis(node);

        final var student = new Student("First", "Last");
        client.jsonSet("student", Path.ROOT_PATH, student);

        System.out.println(client.jsonGet("student"));
    }
}