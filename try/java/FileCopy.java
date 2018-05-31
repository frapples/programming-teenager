import java.io.FileOutputStream;  
import java.io.IOException;  
import java.io.FileInputStream;

public class FileCopy { 
    public static void copy(String from, String to) throws IOException {
            FileInputStream in = new FileInputStream(from);
            FileOutputStream out = new FileOutputStream(to);

            try {
                byte[] buff = new byte[1024];  
                int count;  
                while ((count = in.read(buff)) > 0) {
                    out.write(buff, 0, count);  
                }  
            } finally {
                in.close();  
                out.close();  
            }
    }  
}  
