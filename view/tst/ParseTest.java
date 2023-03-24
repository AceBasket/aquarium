import static org.junit.Assert.assertTrue;
import static org.junit.jupiter.api.Assertions.assertEquals;

import java.io.File;
import java.io.IOException;

import org.junit.jupiter.api.Test;

public class ParseTest {
    @Test
    void parseIpAddress() throws IOException {
        System.out.println("Working Directory = " + System.getProperty("user.dir"));
        File testFile0 = new File("/home/sarah/ENSEIRB_MATMECA/2A/free-Reseaux-1/view/tst/testFilesToParse/0.0.0.0.txt");
        File testFile1 = new File("./testFilesToParse/0.0.0.0.txt");
        assertTrue(testFile0.exists());
        assertTrue(testFile1.exists());
        // assertEquals("0.0.0.0", Parse.parserIP(testFile0));
    }
}
