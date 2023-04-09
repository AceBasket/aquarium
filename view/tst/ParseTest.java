// import static org.junit.Assert.assertThrows;
// import static org.junit.Assert.assertTrue;
// import static org.junit.jupiter.api.Assertions.assertEquals;

// import java.io.File;
// import java.io.IOException;

// import org.junit.jupiter.api.BeforeAll;
// import org.junit.jupiter.api.Test;

// import utils.Parse;

// public class ParseTest {
//     static File testFile0;
//     static File testFile11;
//     static File testFile255;

//     @BeforeAll
//     static void init() {
//         testFile0 = new File("./tst/testFilesToParse/0.txt");
//         testFile11 = new File("./tst/testFilesToParse/11.txt");
//         testFile255 = new File("./tst/testFilesToParse/255.txt");
//         assertTrue(testFile0.exists());
//         assertTrue(testFile11.exists());
//         assertTrue(testFile255.exists());
//     }

//     @Test
//     void testParserIpAddress() throws IOException {
//         assertEquals("", Parse.parserIP(testFile0));

//         assertEquals("11.11.11.11", Parse.parserIP(testFile11));

//         assertEquals("255.255.255.255", Parse.parserIP(testFile255));
//     }

//     @Test
//     void testParserId() throws IOException {
//         assertEquals("N2", Parse.parserID(testFile0));

//         assertEquals("", Parse.parserID(testFile11));

//         assertEquals("NA", Parse.parserID(testFile255));
//     }

//     @Test
//     void testParserPort() throws IOException {
//         assertEquals(23, Parse.parserPort(testFile0));

//         assertEquals(34876, Parse.parserPort(testFile11));

//         // can't test this one because throws Exception on empty string
//         // assertEquals(0, Parse.parserPort(testFile255));
//     }

//     @Test
//     void testParserTimeout() throws IOException {
//         assertEquals(-117, Parse.parserTimeout(testFile0));

//         assertEquals(2, Parse.parserTimeout(testFile11));

//         // can't test this one because throws Exception on empty string
//         // assertEquals(0, Parse.parserTimeout(testFile255));
//     }

//     @Test
//     void testParserResources() throws IOException {
//         assertEquals("blabla", Parse.parserResources(testFile0));

//         assertEquals("", Parse.parserResources(testFile11));

//         assertEquals("0toto", Parse.parserResources(testFile255));
//     }
// }

// /* 
//  * javac -d bin -cp bin:lib/junit-platform-console-standalone-1.9.2.jar src/Parse.java tst/ParseTest.java
//  * java -jar lib/junit-platform-console-standalone-1.9.2.jar --class-path bin --scan-class-path
//  */