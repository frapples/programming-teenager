package io.github.frapples.WebAppTest;

import io.github.frapples.WebAppTest.controller.ControllerInterface;
import org.reflections.Reflections;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.*;

/**
 * Created by computer on 17-7-10.
 */
public class HelloServert extends HttpServlet {
    @Override
    public void service(HttpServletRequest servletRequest, HttpServletResponse response)
            throws ServletException, IOException {
        /* 需要先设置编码，然后获取Writer对象 */
        response.setCharacterEncoding("utf-8");
        servletRequest.setCharacterEncoding("utf-8");
        response.setContentType("text/html;charset=utf-8");

        RequestDecorator request = new RequestDecorator(servletRequest);
        Reflections reflections = new Reflections("io.github.frapples.WebAppTest.controller");
        Set<Class<? extends ControllerInterface>> allClasses =
                reflections.getSubTypesOf(ControllerInterface.class);

        boolean hasController = false;
        for (Class<? extends ControllerInterface> class_ : allClasses) {
            PrintWriter writer = response.getWriter();
            // writer.println(class_.getName());
            if(class_.isAnnotationPresent(Route.class)) {
                Route route = (Route)class_.getAnnotation(Route.class);
                Map<String, String> routeResult = matchRoute(request.getPath(), route.path());
                if (routeResult != null) {
                    try {
                        class_.newInstance().service(request, response, routeResult);
                        hasController = true;
                    } catch (InstantiationException e) {
                        e.printStackTrace();
                    } catch (IllegalAccessException e) {
                        e.printStackTrace();
                    }
                }
            }
        }

        if (!hasController) {
            response.setStatus(404);
        }
    }

    private Map<String, String> matchRoute(String path, String routePath) {
        ArrayList<String> customPath = new ArrayList<String>(
                Arrays.asList(routePath.split("/")));
        customPath.removeIf((s) -> s.equals(""));

        ArrayList<String> urlPath = new ArrayList<String>(
                Arrays.asList(path.split("/")));
        urlPath.removeIf((s) -> s.equals(""));

        if (customPath.size() != urlPath.size()) {
            return null;
        }

       HashMap<String, String> result = new HashMap<String, String>();
       for (int i = 0; i < urlPath.size() && i < customPath.size(); i++) {
           String seg = urlPath.get(i);
           String customSeg = customPath.get(i);

           if (customSeg.length() > 2 &&
                   customSeg.charAt(0) == '{' &&
                   customSeg.charAt(customSeg.length() - 1) == '}') {
               result.put(customSeg.substring(1,  customSeg.length() - 1), seg);
           } else {
               if (!seg.equals(customSeg)) {
                   return null;
               }
           }
       }
       return result;
    }
}
