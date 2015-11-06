/**
 * Created by chenhao on 15/11/6.
 */
Meteor.methods({
    build_ino: function (sourcePath , outputPath, module, user_info) {
        // This method call won't return immediately, it will wait for the
        // asynchronous code to finish, so we call unblock to allow this client
        // to queue other method calls (see Meteor docs)
        this.unblock();
        var future=new Future();
        var command="ino";
        exec(command,function(error,stdout,stderr){
            if(error){
                console.log(error);
                throw new Meteor.Error(500,command+" failed");
            }
            future.return(stdout.toString());
        });
        return future.wait();
    }
});