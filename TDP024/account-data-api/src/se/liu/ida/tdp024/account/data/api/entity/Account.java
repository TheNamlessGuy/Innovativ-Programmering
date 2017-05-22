package se.liu.ida.tdp024.account.data.api.entity;

import java.io.Serializable;

public interface Account extends Serializable {
    public long getID();
    public void setID(long newID);
    
    public String getAccountType();
    public void setAccountType(String newType) throws Exception;
    
    public String getPersonKey();
    public void setPersonKey(String newKey);
    
    public String getBankKey();
    public void setBankKey(String newKey);
    
    public long getHoldings();
    public void setHoldings(long newHolding);
}
