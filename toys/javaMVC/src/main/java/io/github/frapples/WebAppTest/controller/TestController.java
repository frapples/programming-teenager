package io.github.frapples.WebAppTest.controller;

import io.github.frapples.WebAppTest.RequestDecorator;
import io.github.frapples.WebAppTest.Route;

import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Map;

/**
 * Created by computer on 17-7-12.
 */
@Route(path = "test")
public class TestController implements ControllerInterface {
    @Override
    public void service(RequestDecorator request, HttpServletResponse response, Map<String, String> routeVar) throws IOException {
        final PrintWriter writer = response.getWriter();
            try {
                request.getAllHeaders().forEach((k, v) -> writer.println(k + ": " + v));
            } catch (RequestDecorator.CannotAccessHeaderException e) {
                writer.println("Cannot access headers");
            }
    }
}
