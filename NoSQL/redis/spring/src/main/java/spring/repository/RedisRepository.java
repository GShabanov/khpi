package spring.repository;

import java.util.Optional;

import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Repository;
import spring.model.Person;

@Repository
public interface RedisRepository extends CrudRepository<Person, String> {

    Optional<Person> findByFirstName(String name);

}
