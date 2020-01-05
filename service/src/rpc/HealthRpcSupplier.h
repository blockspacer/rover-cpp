//
// Created by Ivan Kishchenko on 2019-06-16.
//

#ifndef ROVER_HEALTHRPCSUPPLIER_H
#define ROVER_HEALTHRPCSUPPLIER_H

#include <rpc/RpcMethod.h>
#include <rpc/json/Helper.h>

BEGIN_DECLARE_DTO(HealthResponse)

__DECLARE_DTO_FIELDS1(std::string, status)
END_DECLARE_DTO


class HealthRpcSupplier : public RpcSupplier<HealthResponse> {
public:
    std::string name() const override {
        return "health";
    }

    HealthResponse exec() const override;
};

#endif //ROVER_HEALTHRPCSUPPLIER_H
