/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package se.liu.ida.tdp024.account.data.impl.db.facade;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.List;
import javax.persistence.EntityManager;
import javax.persistence.LockModeType;
import javax.persistence.Query;
import se.liu.ida.tdp024.account.data.api.entity.Account;
import se.liu.ida.tdp024.account.data.api.entity.Transaction;
import se.liu.ida.tdp024.account.data.api.facade.AccountEntityFacade;
import se.liu.ida.tdp024.account.data.impl.db.entity.TransactionDB;
import se.liu.ida.tdp024.account.util.logger.Logger;
import se.liu.ida.tdp024.account.util.logger.LoggerImpl;
import se.liu.ida.tdp024.account.data.api.facade.TransactionEntityFacade;
import se.liu.ida.tdp024.account.data.impl.db.entity.AccountDB;
import se.liu.ida.tdp024.account.data.impl.db.util.EMF;
import se.liu.ida.tdp024.account.util.logger.Logger.LoggerLevel;

/**
 *
 * @author marle943
 */
public class TransactionEntityFacadeDB implements TransactionEntityFacade {
    
    private static final Logger logger = new LoggerImpl();
    private static final AccountEntityFacade accountEntityFacade = new AccountEntityFacadeDB();
    
    @Override
    public long createTransaction(long accountID, long amount, String type)
    {
        logger.log(LoggerLevel.INFO, "createTransaction(" + Long.toString(accountID) + ", " + Long.toString(amount) + ", " + type + ")");
        
        EntityManager em = EMF.getEntityManager();
        Account acc;
        
        try
        {
            acc = em.find(AccountDB.class, accountID);
            
            em.getTransaction().begin();
            
            em.lock(acc, LockModeType.PESSIMISTIC_WRITE);
            
            Transaction transaction = new TransactionDB();
            transaction.setType(type);
            transaction.setAmount(amount);
            transaction.setCreated(new SimpleDateFormat("yyyy-MM-dd HH:mm:ss:SSS").format(Calendar.getInstance().getTime()));
            transaction.setAccount(acc);
            
            if (type.equals("DEBIT") && acc.getHoldings() >= amount) {
                acc.setHoldings(acc.getHoldings() - amount);
                transaction.setStatus("OK");
            } else if (type.equals(("CREDIT"))) {
                acc.setHoldings(acc.getHoldings() + amount);
                transaction.setStatus("OK");
            } else {
                transaction.setStatus("FAILED");
            }
            
            em.merge(acc);
            em.persist(transaction);
            
            em.getTransaction().commit();
            
            logger.log(LoggerLevel.INFO, "createTransaction(" + Long.toString(accountID) + ", " + Long.toString(amount) + ", " + type + ") => Transaction created (" + transaction.getStatus() + ")");
            return transaction.getId();
        }
        catch (Exception e)
        {
            logger.log(LoggerLevel.ERROR, "createTransaction(" + Long.toString(accountID) + ", " + Long.toString(amount) + ", " + type + ") => ERROR FOUND: " + e.getMessage());
            return -1;
        }
        finally
        {
            if (em.getTransaction().isActive()) {
                em.getTransaction().rollback();
            }
            em.close();
        }
    }
    
    @Override
    public List<Transaction> findTransactionsFor(long id)
    {
        logger.log(LoggerLevel.INFO, "findTransactionFor(" + Long.toString(id) + ")");
        
        EntityManager em = EMF.getEntityManager();
        Account acc;
        
        try {
            acc = em.find(AccountDB.class, id);
            
            Query q = em.createQuery("SELECT t FROM TransactionDB t WHERE t.account = :acc ");
            q.setParameter("acc", acc);
            
            List<Transaction> lst = q.getResultList();
            
            String log = "findTransactionFor(" + Long.toString(id) + ") => Got transactions: [";
            for (Transaction t : lst) {
                log += Long.toString(t.getId());
                if (t != lst.get(lst.size() - 1))
                    log += ",";
            }
            log += "]";
            
            logger.log(LoggerLevel.INFO, log);
            return lst;
        } catch (Exception e) {
            logger.log(LoggerLevel.ERROR, "findTransactionFor(" + Long.toString(id) + ") => ERROR FOUND: " + e.getMessage());
            return new ArrayList<Transaction>();
        } finally {
            em.close();
        }
    }
}
