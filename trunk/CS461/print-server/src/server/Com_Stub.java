// Stub class generated by rmic, do not edit.
// Contents subject to change without notice.

package server;

public final class Com_Stub
    extends java.rmi.server.RemoteStub
    implements common.ICom, java.rmi.Remote
{
    private static final long serialVersionUID = 2;
    
    private static java.lang.reflect.Method $method_login_0;
    private static java.lang.reflect.Method $method_ping_1;
    private static java.lang.reflect.Method $method_signup_2;
    
    static {
	try {
	    $method_login_0 = common.ICom.class.getMethod("login", new java.lang.Class[] {java.lang.String.class, java.lang.String.class});
	    $method_ping_1 = common.ICom.class.getMethod("ping", new java.lang.Class[] {});
	    $method_signup_2 = common.ICom.class.getMethod("signup", new java.lang.Class[] {dataset.User.class});
	} catch (java.lang.NoSuchMethodException e) {
	    throw new java.lang.NoSuchMethodError(
		"stub class initialization failed");
	}
    }
    
    // constructors
    public Com_Stub(java.rmi.server.RemoteRef ref) {
	super(ref);
    }
    
    // methods from remote interfaces
    
    // implementation of login(String, String)
    public dataset.User login(java.lang.String $param_String_1, java.lang.String $param_String_2)
	throws java.rmi.RemoteException
    {
	try {
	    Object $result = ref.invoke(this, $method_login_0, new java.lang.Object[] {$param_String_1, $param_String_2}, 8255974224146657066L);
	    return ((dataset.User) $result);
	} catch (java.lang.RuntimeException e) {
	    throw e;
	} catch (java.rmi.RemoteException e) {
	    throw e;
	} catch (java.lang.Exception e) {
	    throw new java.rmi.UnexpectedException("undeclared checked exception", e);
	}
    }
    
    // implementation of ping()
    public java.lang.String ping()
	throws java.rmi.RemoteException
    {
	try {
	    Object $result = ref.invoke(this, $method_ping_1, null, -5977121354941433790L);
	    return ((java.lang.String) $result);
	} catch (java.lang.RuntimeException e) {
	    throw e;
	} catch (java.rmi.RemoteException e) {
	    throw e;
	} catch (java.lang.Exception e) {
	    throw new java.rmi.UnexpectedException("undeclared checked exception", e);
	}
    }
    
    // implementation of signup(User)
    public boolean signup(dataset.User $param_User_1)
	throws java.rmi.RemoteException
    {
	try {
	    Object $result = ref.invoke(this, $method_signup_2, new java.lang.Object[] {$param_User_1}, -2606815845293560872L);
	    return ((java.lang.Boolean) $result).booleanValue();
	} catch (java.lang.RuntimeException e) {
	    throw e;
	} catch (java.rmi.RemoteException e) {
	    throw e;
	} catch (java.lang.Exception e) {
	    throw new java.rmi.UnexpectedException("undeclared checked exception", e);
	}
    }
}
