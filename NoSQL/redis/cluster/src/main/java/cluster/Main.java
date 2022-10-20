package cluster;

import java.util.HashSet;
import java.util.Set;

import redis.clients.jedis.HostAndPort;
import redis.clients.jedis.JedisCluster;

public class Main {

    public static void main(String[] args) {
        final Set<HostAndPort> nodes = new HashSet<>();
        nodes.add(new HostAndPort("192.168.0.6", 6373));
        nodes.add(new HostAndPort("192.168.0.6", 6374));
        nodes.add(new HostAndPort("192.168.0.6", 6375));
        nodes.add(new HostAndPort("192.168.0.6", 6376));
        nodes.add(new HostAndPort("192.168.0.6", 6377));
        nodes.add(new HostAndPort("192.168.0.6", 6378));

        JedisCluster jedisCluster = new JedisCluster(nodes);

        jedisCluster.set("key", "value");
        System.out.println(jedisCluster.get("key"));
    }
}