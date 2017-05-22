package se.liu.ida.tdp024.account.data.impl.db.entity;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GenerationType;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;
import javax.persistence.Table;
import se.liu.ida.tdp024.account.data.api.entity.Account;

@Entity
@Table(name = "ACCOUNTDB")
public class AccountDB implements Account {
    
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "id")
    private long id;
    
    private String personKey;
    private String bankKey;
    
    private String accountType;
    
    private long holdings;
    
    @Override
    public long getID()
    {
        return id;
    }
    
    @Override
    public void setID(long newID)
    {
        id = newID;
    }
    
    @Override
    public String getAccountType()
    {
        return accountType;
    }
    
    @Override
    public void setAccountType(String newType) throws Exception
    {
        if (newType.equals("CHECK") || newType.equals("SAVINGS")) {
            accountType = newType;
        } else {
            throw new Exception("Type has to be CHECK or SAVINGS");
        }
    }
    
    @Override
    public String getPersonKey()
    {
        return personKey;
    }
    
    @Override
    public void setPersonKey(String newKey)
    {
        personKey = newKey;
    }
    
    @Override
    public String getBankKey()
    {
        return bankKey;
    }
    
    @Override
    public void setBankKey(String newKey)
    {
        bankKey = newKey;
    }
    
    @Override
    public long getHoldings()
    {
        return holdings;
    }
    
    @Override
    public void setHoldings(long newHolding)
    {
        holdings = newHolding;
    }
}
