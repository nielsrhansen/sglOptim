#ifndef OBJECTIVEFUNCTIONTYPE_H_
#define OBJECTIVEFUNCTIONTYPE_H_

template< typename E >
class ObjectiveFunctionType {

public:

	typedef typename E::data_type data_type;
	typedef E instance_type;

	data_type const& data;

	ObjectiveFunctionType(data_type const& data) : data(data) {}

	instance_type create_instance(sgl::DimConfig const& dim_config) const {
		return instance_type(data, dim_config);
	}

};

#endif /* OBJECTIVEFUNCTIONTYPE_H_ */
