package io.github.frapples.WebAppTest;

import io.github.frapples.WebAppTest.utils.DefaultArrayList;

import javax.servlet.http.HttpServletRequest;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Map;

/**
 * Created by computer on 17-7-11.
 */
public class RequestDecorator {
    public static class CannotAccessHeaderException extends Exception {
    }

    public HttpServletRequest servletRequest;
    RequestDecorator(HttpServletRequest request) {
        servletRequest = request;
    }

    public String getPath() {
        return servletRequest.getPathInfo() == null ? "" : servletRequest.getPathInfo();
    }
    public DefaultArrayList<String> getRoutes() {
        String[] routes = getPath().split("/");
        DefaultArrayList<String> results = new DefaultArrayList<String>();
        int len = routes.length;
        for (String name : routes) {
            if (!name.equals("")) {
                results.add(name);
            }
        }
        return results;
    }

    public int getIntPara(String name, int default_) {
        try {
            return Integer.parseInt(servletRequest.getParameter(name));
        } catch (NumberFormatException e) {
            return default_;
        }
    }

    public int getIntPara(String name) {
        return getIntPara(name, 0);
    }

    public String getStrPara(String name) {
        String s = servletRequest.getParameter(name);
        return s != null ? s : "";
    }

    public Map<String, String> getAllHeaders() throws CannotAccessHeaderException {
        Enumeration<String> headNames = servletRequest.getHeaderNames();

        if (headNames == null) {
            throw new CannotAccessHeaderException();
        } else {
            Map<String, String> result = new HashMap<String, String>();
            while (headNames.hasMoreElements()) {
                String name = headNames.nextElement();
                result.put(name, servletRequest.getHeader(name));
            }
            return result;
        }
    }
}
