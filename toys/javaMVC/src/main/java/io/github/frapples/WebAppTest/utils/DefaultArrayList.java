package io.github.frapples.WebAppTest.utils;

import java.util.ArrayList;

/**
 * Created by computer on 17-7-11.
 */
public class DefaultArrayList<T> extends ArrayList<T> {
    public DefaultArrayList() {
        super();
    }

    public T get(int i, T default_) {
        try {
            return get(i);
        } catch (IndexOutOfBoundsException e) {
            return default_;
        }
    }
}
