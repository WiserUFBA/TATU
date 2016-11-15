# TATU
Tatu Repository

## The tatu protocol
The protocol is intendend to control the requisitions and responses in an IoT enviromment, and it's done with the use of following methods:

### Methods
* GET -> Requests an atrribute value
⋅⋅* Literal structure: {"CODE":"GET","DATA":"<datatype>","VAR":"<symbol>"}
⋅⋅* Device structure: "GET <datatype> <symbol>"
* SET -> Changes an atrribute value
⋅⋅* Literal structure: {"CODE":"SET","DATA":"<datatype>","VAR":"<symbol>","VAl","<value>"}
 ⋅⋅* Device structure: "SET <datatype> <symbol> <value>"
* POST -> Is the response to a requisition