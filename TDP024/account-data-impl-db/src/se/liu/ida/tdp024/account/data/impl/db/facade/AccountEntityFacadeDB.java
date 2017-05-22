package se.liu.ida.tdp024.account.data.impl.db.facade;

import java.util.ArrayList;
import java.util.List;
import javax.persistence.EntityManager;
import javax.persistence.LockModeType;
import javax.persistence.Query;
import se.liu.ida.tdp024.account.data.api.entity.Account;
import se.liu.ida.tdp024.account.data.impl.db.entity.AccountDB;
import se.liu.ida.tdp024.account.data.api.facade.AccountEntityFacade;
import se.liu.ida.tdp024.account.data.impl.db.util.EMF;
import se.liu.ida.tdp024.account.util.logger.Logger;
import se.liu.ida.tdp024.account.util.logger.Logger.LoggerLevel;
import se.liu.ida.tdp024.account.util.logger.LoggerImpl;

public class AccountEntityFacadeDB implements AccountEntityFacade {
    
    private static final Logger logger = new LoggerImpl();
    
    @Override
    public long createAccount(String personKey, String bankKey, String accountType)
    {
        logger.log(LoggerLevel.INFO, "createAccount(" + personKey + ", " + bankKey + ", " + accountType + ")");
        
        /*if (!canCreateAccount(personKey, bankKey, accountType)) {
            return -1;
        }*/
        
        EntityManager em = EMF.getEntityManager();
        
        try
        {
            em.getTransaction().begin();
            
            Account account = new AccountDB();
            account.setBankKey(bankKey);
            account.setPersonKey(personKey);
            account.setAccountType(accountType);
            account.setHoldings(0);
            
            em.persist(account);
            
            em.getTransaction().commit();
            
            logger.log(LoggerLevel.INFO, "createAccount(" + personKey + ", " + bankKey + ", " + accountType + ") => Account created");
            return account.getID();
        }
        catch (Exception e)
        {
            logger.log(LoggerLevel.ERROR, "createAccount(" + personKey + ", " + bankKey + ", " + accountType + ") => ERROR FOUND: " + e.getMessage());
            return -1;
        }
        finally
        {
            if (em.getTransaction().isActive())
                em.getTransaction().rollback();
            
            em.close();
        }
    }
    
    /*@Override
    public Account getAccount(long id)
    {
        EntityManager em = EMF.getEntityManager();
        
        try {
            return em.find(AccountDB.class, id);
        } catch (Exception e) {
            logger.log(e);
            return null;
        } finally {
            em.close();
        }
    }*/
    
    @Override
    public List<Account> findAccountsFor(String personKey)
    {
        logger.log(LoggerLevel.INFO, "findAccountsFor(" + personKey + ")");
        EntityManager em = EMF.getEntityManager();
        
        try {
            Query q = em.createQuery("SELECT a FROM AccountDB a WHERE a.personKey = :inputPersonKey ");
            
            List<Account> lst = q.setParameter("inputPersonKey", personKey).getResultList();
            
            String log = "findAccountsFor(" + personKey + ") => Found accounts: [";
            for (Account a : lst) {
                log += Long.toString(a.getID());
                if (a != lst.get(lst.size() - 1)) {
                    log += ",";
                }
            }
            log += "]";
            
            logger.log(LoggerLevel.INFO, log);
            return lst;
        } catch (Exception e) {
            logger.log(LoggerLevel.ERROR, "findAccountsFor(" + personKey + ") => ERROR FOUND: " + e.getMessage());
            return new ArrayList<Account>();
        } finally {
            em.close();
        }
    }
    
    /*@Override
    public boolean canFindAccount(long id)
    {
        logger.log(LoggerLevel.INFO, "canFindAccount(" + Long.toString(id) + ")");
        
        EntityManager em = EMF.getEntityManager();
        
        try {
            Query q = em.createQuery("SELECT a FROM AccountDB a WHERE a.id = :accid");
            q.setParameter("accid", id);
            
            if (q.getResultList().isEmpty()) {
                logger.log(LoggerLevel.INFO, "canFindAccount(" + Long.toString(id) + ") => Could not find account");
                return false;
            }
            logger.log(LoggerLevel.INFO, "canFindAccount(" + Long.toString(id) + ") => Found account");
            return true;
        } catch (Exception e) {
            logger.log(LoggerLevel.ERROR, "canFindAccount(" + Long.toString(id) + ") => ERROR FOUND: " + e.getMessage());
            return false;
        } finally {
            em.close();
        }
    }
    
    @Override
    public boolean hasEnoughHoldings(long id, long amount)
    {
        logger.log(LoggerLevel.INFO, "hasEnoughHoldings(" + Long.toString(id) + ", " + Long.toString(amount) + ")");
        
        EntityManager em = EMF.getEntityManager();
        
        try {
            Query q = em.createQuery("SELECT a FROM AccountDB a WHERE a.id = :accid");
            q.setParameter("accid", id);
            
            Account a = (Account)q.getSingleResult();
            
            if (a.getHoldings() < amount) {
                logger.log(LoggerLevel.INFO, "hasEnoughHoldings(" + Long.toString(id) + ", " + Long.toString(amount) + ") => Account does not have enough holdings");
                return false;
            }
            logger.log(LoggerLevel.INFO, "hasEnoughHoldings(" + Long.toString(id) + ", " + Long.toString(amount) + ") => Account has enough holdings");
            return true;
        } catch (Exception e) {
            logger.log(LoggerLevel.ERROR, "hasEnoughHoldings(" + Long.toString(id) + ", " + Long.toString(amount) + ") => ERROR FOUND: " + e.getMessage());
            return false;
        } finally {
            em.close();
        }
    }
    
    @Override
    public boolean changeAccountHoldings(long accountID, long amount, String type)
    {
        logger.log(LoggerLevel.INFO, "changeAccountHoldings(" + Long.toString(accountID) + ", " + Long.toString(amount) + ", " + type + ")");
        
        
        if (!canFindAccount(accountID)) {
            logger.log(LoggerLevel.INFO, "changeAccountHoldings(" + Long.toString(accountID) + ", " + Long.toString(amount) + ", " + type + ") => Account does not exist");
            return false;
        } else if (type.equals("DEBIT") && !hasEnoughHoldings(accountID, amount)) {
            logger.log(LoggerLevel.INFO, "changeAccountHoldings(" + Long.toString(accountID) + ", " + Long.toString(amount) + ", " + type + ") => Account does not have enough holdings");
            return false;
        } else if (amount < 0) {
            logger.log(LoggerLevel.INFO, "changeAccountHoldings(" + Long.toString(accountID) + ", " + Long.toString(amount) + ", " + type + ") => Trying to change with amount being less than 0");
            return false;
        }
        
        EntityManager em = EMF.getEntityManager();
        Account a;
        
        try {
            Query q = em.createQuery("SELECT a FROM AccountDB a WHERE a.id = :accid");
            q.setParameter("accid", accountID);
            
            a = (Account)q.getSingleResult();
            em.getTransaction().begin();
            //em.lock(a, LockModeType.PESSIMISTIC_READ);
            
            if (type.equals("CREDIT"))
                a.setHoldings(a.getHoldings() + amount);
            else
                a.setHoldings(a.getHoldings() - amount);
            
            em.merge(a);
            em.getTransaction().commit();
            logger.log(LoggerLevel.INFO, "changeAccountHoldings(" + Long.toString(accountID) + ", " + Long.toString(amount) + ", " + type + ") => Went through");
            return true;
        } catch (Exception e) {
            logger.log(LoggerLevel.ERROR, "changeAccountHoldings(" + Long.toString(accountID) + ", " + Long.toString(amount) + ", " + type + ") => ERROR FOUND: " + e.getMessage());
            return false;
        } finally {
            if (em.getTransaction().isActive()) {
                em.getTransaction().rollback();
            }
            
            em.close();
        }
    }*/
}
