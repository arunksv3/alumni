jQuery(document).ready(function($) { 
    $.validator.addMethod("alphaExp", function(value, element) {
        return this.optional(element) || /^[a-zA-Z0-9\s]+$/.test(value);
    });
    $.validator.addMethod("emailExp", function(value, element) {      
        var re = new RegExp(/^(("[\w-\s]+")|([\w-]+(?:\.[\w-]+)*)|("[\w-\s]+")([\w-]+(?:\.[\w-]+)*))(@((?:[\w-]+\.)*\w[\w-]{0,66})\.([a-z]{2,6}(?:\.[a-z]{2})?)$)|(@\[?((25[0-5]\.|2[0-4][0-9]\.|1[0-9]{2}\.|[0-9]{1,2}\.))((25[0-5]|2[0-4][0-9]|1[0-9]{2}|[0-9]{1,2})\.){2}(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[0-9]{1,2})\]?$)/i);
        return this.optional(element) || re.test(value);
    });
    $.validator.addMethod("numericExp", function(value, element) {
        return this.optional(element) || /^[0-9]+$/.test(value);
    });
    $.validator.addMethod("zipcode", function(value, element) {
        return this.optional(element) || (/^\d{5}$/.test(value) && value != "00000" && value.substring(0,3) != "000");
    });
    $.validator.addMethod("salaryexp", function(value, element) {
        return this.optional(element) || /^[0-9,]+$/.test(value);
    });
    $.validator.addMethod("phoneexp", function(value, element) {
        return this.optional(element) || /^[0-9-().]+$/.test(value);
    });
    $.validator.addMethod("faxexp", function(value, element) {
        return this.optional(element) || /^[0-9-().]+$/.test(value);
    });
}); 
