package io.github.frapples.WebAppTest.controller;

import com.google.gson.Gson;
import io.github.frapples.WebAppTest.RequestDecorator;
import io.github.frapples.WebAppTest.Route;

import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.util.Map;

class Person {
    private int id;
    private String name;
    Person(int id, String name) {
        this.id = id;
        this.name = name;
    }
}

/**
 * Created by computer on 17-7-11.
 */
@Route(path = "/jsonapi/{name}")
public class JsonApiController implements ControllerInterface {
    @Override
    public void service(RequestDecorator request, HttpServletResponse response, Map<String, String> routeVar) throws IOException {
        response.setContentType("text/json;charset=utf-8");
        // response.setStatus(404);
        Person p = new Person(request.getIntPara("id"), routeVar.get("name"));
        response.getWriter().print((new Gson()).toJson(p));
    }
}
