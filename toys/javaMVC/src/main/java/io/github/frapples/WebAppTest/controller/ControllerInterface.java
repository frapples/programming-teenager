package io.github.frapples.WebAppTest.controller;

import io.github.frapples.WebAppTest.RequestDecorator;

import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.util.Map;

/**
 * Created by computer on 17-7-11.
 */
public interface ControllerInterface {
    void service(RequestDecorator request, HttpServletResponse response, Map<String, String> routeVar) throws IOException;
}
