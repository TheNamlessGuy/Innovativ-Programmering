/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package se.liu.ida.tdp024.account.data.impl.db.entity;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.ManyToOne;
import javax.persistence.Table;
import se.liu.ida.tdp024.account.data.api.entity.Account;
import se.liu.ida.tdp024.account.data.api.entity.Transaction;

/**
 *
 * @author marle943
 */
@Entity
@Table(name="TRANSACTIONDB")
public class TransactionDB implements Transaction {
    
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name="id")
    private long id;
    
    private String type;
    private long amount;
    private String created;
    private String status;
    
    @ManyToOne(targetEntity = AccountDB.class)
    private Account account;
    
    @Override
    public long getId()
    {
        return id;
    }
    
    @Override
    public void setId(long newID)
    {
        id = newID;
    }
    
    @Override
    public String getType()
    {
        return type;
    }
    
    @Override
    public void setType(String newType) throws Exception
    {
        if (newType.equals("DEBIT") || newType.equals("CREDIT")) {
            type = newType;            
        } else {
            throw new Exception("Transaction type needs to be DEBIT or CREDIT");
        }
    }
    
    @Override
    public long getAmount()
    {
        return amount;
    }
    
    @Override
    public void setAmount(long newAmount)
    {
        amount = newAmount;
    }
    
    @Override
    public String getCreated()
    {
        return created;
    }
    
    @Override
    public void setCreated(String newDate)
    {
        created = newDate;
    }
    
    @Override
    public String getStatus()
    {
        return status;
    }
    
    @Override
    public void setStatus(String newStatus) throws Exception
    {
        if (newStatus.equals("OK") || newStatus.equals("FAILED")) {
            status = newStatus;
        } else {
            throw new Exception("Status needs to be OK or FAILED");
        }        
    }
    
    @Override
    public Account getAccount()
    {
        return account;
    }
    
    @Override
    public void setAccount(Account newAccount)
    {
        account = newAccount;
    }
}
