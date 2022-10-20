package spring.endpoint;

import javax.annotation.PostConstruct;

import lombok.RequiredArgsConstructor;
import org.springframework.cache.annotation.Cacheable;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import spring.model.Person;
import spring.repository.RedisRepository;

@RestController
@RequestMapping("students/{name}")
@RequiredArgsConstructor
public class Endpoint {

    private final RedisRepository repository;

    @PostConstruct
    public void postConstruct() {
        repository.save(new Person("1", "First", "Last"));
    }

    @GetMapping
    @Cacheable(value = "studentCache")
    public Person getItemForId(@PathVariable String name) {
        System.out.println("Fetching from DB");

        return repository.findByFirstName(name).orElseThrow();
    }
}
