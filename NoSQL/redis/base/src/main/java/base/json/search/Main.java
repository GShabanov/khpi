package base.json.search;

import java.util.List;

import base.json.Student;
import redis.clients.jedis.HostAndPort;
import redis.clients.jedis.UnifiedJedis;
import redis.clients.jedis.json.Path;
import redis.clients.jedis.search.Document;
import redis.clients.jedis.search.IndexDefinition;
import redis.clients.jedis.search.IndexOptions;
import redis.clients.jedis.search.Query;
import redis.clients.jedis.search.Schema;
import redis.clients.jedis.search.SearchResult;

public class Main {

    public static void main(String[] args) {
        final var node = new HostAndPort("192.168.0.6", 6379);
        UnifiedJedis client = new UnifiedJedis(node);

        final var student = new Student("First", "Last");
        client.jsonSet("student:123", Path.ROOT_PATH, student);

        Schema schema = new Schema().addTextField("$.firstName", 1.0)
                                    .addTextField("$.lastName", 1.0);
        IndexDefinition rule = new IndexDefinition(IndexDefinition.Type.JSON).setPrefixes("student:");
        client.ftCreate("student-index", IndexOptions.defaultOptions().setDefinition(rule), schema);

        Query q = new Query("@\\$\\" + ".firstName:first*");
        SearchResult search = client.ftSearch("student-index", q);

        List<Document> docs = search.getDocuments();
        for (Document doc : docs) {
            System.out.println(doc.getProperties());
        }
    }
}
