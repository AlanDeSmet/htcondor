

        /**
         * AviaryQueryService.cpp
         *
         * This file was auto-generated from WSDL for "AviaryQueryService|http://grid.redhat.com/aviary-query/" service
         * by the Apache Axis2 version: 1.0  Built on : Jan 09, 2011 (11:40:28 EST)
         *  AviaryQueryService
         */

        #include "AviaryQueryServiceSkeleton.h"
        #include "AviaryQueryService.h"  
        #include <ServiceSkeleton.h>
        #include <stdio.h>
        #include <axis2_svc.h>
        #include <Environment.h>
        #include <axiom_soap.h>


        using namespace wso2wsf;
        
        using namespace com_redhat_grid_aviary_query;
        

        /** Load the service into axis2 engine */
        WSF_SERVICE_INIT(AviaryQueryService)

          
         /**
          * function to free any soap input headers
          */
         AviaryQueryService::AviaryQueryService()
	{
          skel = wsfGetAviaryQueryServiceSkeleton();
    }


	void WSF_CALL
	AviaryQueryService::init()
	{

      return;
	}


	AviaryQueryService::~AviaryQueryService()
	{
    }


     

     




	/*
	 * This method invokes the right service method
	 */
	OMElement* WSF_CALL
	AviaryQueryService::invoke(OMElement *omEle, MessageContext *msgCtx)
	{
         /* Using the function name, invoke the corresponding method
          */

          axis2_op_ctx_t *operation_ctx = NULL;
          axis2_op_t *operation = NULL;
          axutil_qname_t *op_qname = NULL;
          axis2_char_t *op_name = NULL;
          axis2_msg_ctx_t *in_msg_ctx = NULL;
          
          axiom_soap_envelope_t *req_soap_env = NULL;
          axiom_soap_header_t *req_soap_header = NULL;
          axiom_soap_envelope_t *res_soap_env = NULL;
          axiom_soap_header_t *res_soap_header = NULL;

          axiom_node_t *ret_node = NULL;
          axiom_node_t *input_header = NULL;
          axiom_node_t *output_header = NULL;
          axiom_node_t *header_base_node = NULL;
          axis2_msg_ctx_t *msg_ctx = NULL;
          axiom_node_t* content_node = omEle->getAxiomNode();

          
            com_redhat_grid_aviary_common::FindJobResponse* ret_val1;
            com_redhat_grid_aviary_common::FindJob* input_val1;
            
       
          msg_ctx = msgCtx->getAxis2MessageContext();
          operation_ctx = axis2_msg_ctx_get_op_ctx(msg_ctx, Environment::getEnv());
          operation = axis2_op_ctx_get_op(operation_ctx, Environment::getEnv());
          op_qname = (axutil_qname_t *)axis2_op_get_qname(operation, Environment::getEnv());
          op_name = axutil_qname_get_localpart(op_qname, Environment::getEnv());

          if (op_name)
          {
               

                if ( axutil_strcmp(op_name, "findJob") == 0 )
                {

                    
                    input_val1 =
                        
                        new com_redhat_grid_aviary_common::FindJob();
                        if( AXIS2_FAILURE ==  input_val1->deserialize(&content_node, NULL, false))
                        {
                                        
                            AXIS2_ERROR_SET(Environment::getEnv()->error, AXIS2_ERROR_DATA_ELEMENT_IS_NULL, AXIS2_FAILURE);
                            AXIS2_LOG_ERROR( Environment::getEnv()->log, AXIS2_LOG_SI, "NULL returned from the com_redhat_grid_aviary_common::FindJob_deserialize: "
                                        "This should be due to an invalid XML");
                            return NULL;      
                        }
                        
                        //AviaryQueryServiceSkeleton skel;
                        ret_val1 =  skel->findJob(msgCtx ,input_val1);
                    
                        if ( NULL == ret_val1 )
                        {
                            
                                delete input_val1;
                            
                            return NULL; 
                        }
                        ret_node = 
                                            ret_val1->serialize(NULL, NULL, AXIS2_TRUE, NULL, NULL);
                                            delete ret_val1;
                                        
                                            delete input_val1;
                                        

                        return new OMElement(NULL,ret_node);
                    

                    /* since this has no output params it just returns NULL */                    
                    

                }
             
             }
            
          AXIS2_LOG_ERROR(Environment::getEnv()->log, AXIS2_LOG_SI, "AviaryQueryService service ERROR: invalid OM parameters in request\n");
          return NULL;
    }

    OMElement* WSF_CALL
    AviaryQueryService::onFault(OMElement* omEle)
	{
		axiom_node_t *error_node = NULL;
		axiom_element_t *error_ele = NULL;
        axutil_error_codes_t error_code;
        axiom_node_t *node = omEle->getAxiomNode();
        error_code = (axutil_error_codes_t)Environment::getEnv()->error->error_number;

        if(error_code <= AVIARYQUERYSERVICESKELETON_ERROR_NONE ||
                error_code >= AVIARYQUERYSERVICESKELETON_ERROR_LAST )
        {
            error_ele = axiom_element_create(Environment::getEnv(), node, "fault", NULL,
                            &error_node);
            axiom_element_set_text(error_ele, Environment::getEnv(), "AviaryQueryService|http://grid.redhat.com/aviary-query/ failed",
                            error_node);
        }
        

		return new OMElement(NULL,error_node);
	}

    

