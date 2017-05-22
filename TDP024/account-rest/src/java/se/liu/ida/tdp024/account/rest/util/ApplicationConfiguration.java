package se.liu.ida.tdp024.account.rest.util;

import com.sun.jersey.api.core.PackagesResourceConfig;
import javax.ws.rs.ApplicationPath;

@ApplicationPath("/")
public class ApplicationConfiguration extends PackagesResourceConfig {
    
    public ApplicationConfiguration() {
        super("se.liu.ida.tdp024.account.rest.service");
    }
    
}
