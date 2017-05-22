package se.liu.ida.tdp024.account.logic.api.facade;

import java.util.List;
import se.liu.ida.tdp024.account.data.api.entity.Transaction;
/**
 *
 * @author marle943
 */
public interface TransactionLogicFacade {
    public long createTransaction(long accountID, long amount, String type);
    
    public List<Transaction> findTransactionsFor(long id);
}
