package se.liu.ida.tdp024.account.data.api.entity;

import java.io.Serializable;

public interface Transaction extends Serializable {
    public long getId();
    public void setId(long newID);
    
    public String getType();
    public void setType(String newType) throws Exception;
    
    public long getAmount();
    public void setAmount(long newAmount);
    
    public String getCreated();
    public void setCreated(String newDate);
    
    public String getStatus();
    public void setStatus(String newStatus) throws Exception;
       
    public Account getAccount();
    public void setAccount(Account newAccount);
    
    //public Account getAccount();   
}
