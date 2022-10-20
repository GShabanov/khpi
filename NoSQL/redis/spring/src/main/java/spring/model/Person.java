package spring.model;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
import org.springframework.data.annotation.Id;
import org.springframework.data.redis.core.RedisHash;
import org.springframework.data.redis.core.index.Indexed;

@RedisHash("Person")
@Data
@AllArgsConstructor
@NoArgsConstructor
public class Person {

    @Id
    private String id;
    @Indexed
    private String firstName;
    @Indexed
    private String lastName;
}
