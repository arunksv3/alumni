jQuery(document).ready(function () {
    $("#edit-submit").click(function (e) { 
        e.preventDefault();
        var all_answered = true; 
        
        $("input:radio").each(function () {
            var name = $(this).attr("name");
            if ($("input:radio[name=" + name + "]:checked").length == 0) {
                all_answered = false;
                $(this).focus();	
            }
            if(all_answered == false){
                return false;
            }
        });
				
        if (all_answered == false) {
            alert('You need to select answers for all questions to submit the test');
        }else{
            var input = $("<input>")
               .attr("type", "hidden")
               .attr("name", "op").val("Submit Test");
            $( '#psychometrics-test' ).append($(input));
            $( "#psychometrics-test" ).submit();
        }
    })
});