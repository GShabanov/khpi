package org.example;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.testcontainers.containers.GenericContainer;
import org.testcontainers.junit.jupiter.Container;
import org.testcontainers.junit.jupiter.Testcontainers;
import org.testcontainers.utility.DockerImageName;
import redis.clients.jedis.Jedis;

@Testcontainers
class TaxiTest {

    private static final DockerImageName REDIS = DockerImageName.parse("redis/redis-stack-server");

    @Container
    @SuppressWarnings("resource")
    private final GenericContainer<?> redis = new GenericContainer<>(REDIS).withExposedPorts(6379);

    private Taxi unit;

    @BeforeEach
    public void beforeAll() {
        redis.stop();
        redis.start();
        unit = new Taxi(new Jedis("127.0.0.1", redis.getFirstMappedPort()));
    }

    @Test
    public void case0() {
        unit.carRegistered("12.0", "13.0", "car");
        unit.carRegistered("14.0", "15.0", "car1");

        var result = unit.locateCar("car");

        assertNotNull(result);
        assertTrue(result.contains("12.0"));
        assertTrue(result.contains("13.0"));
        assertFalse(result.contains("14.0"));
        assertFalse(result.contains("15.0"));
    }

    @Test
    public void case1() {
        unit.carRegistered("12.0", "13.0", "car1");
        unit.carRegistered("13.0", "11.0", "car2");
        unit.carTaken("car2");

        var result = unit.getFreeCars();

        assertNotNull(result);
        assertTrue(result.contains("car1"));
        assertFalse(result.contains("car2"));
    }

    @Test
    public void case2() {
        unit.scoreCalculated("username1", "123");
        unit.scoreCalculated("username", "321");

        for (int i = 0; i < 3; i++) {
            var result = unit.getBestCustomer();

            assertNotNull(result);
            assertTrue(result.contains("username"));
            assertFalse(result.contains("username1"));
        }
    }

    @Test
    public void case3() {
        unit.orderReceived("order1");
        unit.orderReceived("order2");
        unit.orderReceived("order3");

        asserContainsOrder("order1", "order2", "order3");
        asserContainsOrder("order2", "order1", "order3");
        asserContainsOrder("order3", "order1", "order2");
    }

    @Test
    public void case4() {
        unit.userRegistered("username", "user", "23");
        unit.userRegistered("username1", "user1", "24");

        var result = unit.getUser("username");

        assertNotNull(result);
        assertTrue(result.contains("user"));
        assertTrue(result.contains("23"));
        assertFalse(result.contains("user1"));
    }

    private void asserContainsOrder(String order, String... otherOrders) {
        var result1 = unit.selectOder();

        assertNotNull(result1);
        assertTrue(result1.contains(order));

        for (String other: otherOrders) {
            assertFalse(result1.contains(other));
        }
    }
}