package io.github.frapples.WebAppTest;

import java.lang.annotation.*;

/**
 * Created by computer on 17-7-11.
 */
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface Route {
    String path();
}
