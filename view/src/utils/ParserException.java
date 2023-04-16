package utils;

public class ParserException extends Exception {
    // class for exceptions thrown by the parser
    public ParserException(String message) {
        super(message);
    }

    public ParserException(String message, Throwable cause) {
        super(message, cause);
    }

    public ParserException(Throwable cause) {
        super(cause);
    }

    public ParserException() {
        super();
    }
}
