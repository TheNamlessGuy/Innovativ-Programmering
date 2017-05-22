package se.liu.ida.tdp024.account.data.api.facade;

import se.liu.ida.tdp024.account.data.api.entity.Transaction;
import java.util.List;

public interface TransactionEntityFacade {
    public long createTransaction(long accountID, long amount, String type);
    
    public List<Transaction> findTransactionsFor(long id);
    
    //public Transaction getTransaction(long id);
}
